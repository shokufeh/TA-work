#include "fsp_driver.hpp"

/* Inline utilities. */
#include "utils.hpp"

/* Lts definitions and operations. */
#include "lts.hpp"

/* Serialization and deserialization support. */
#include "serializer.hpp"

/* Interactive shell */
#include "shell.hpp"

/* Preprocessor */
#include "preproc.hpp"

/* Bison generated parser. */
#include "fsp_parser.hpp"

/* The parse tree structures. */
#include "tree.hpp"

/* Some helper routines. */
#include "helpers.hpp"

#include <queue>

using namespace std;


//#define DEBUG
#ifdef DEBUG
#define IFD(x) x
#else
#define IFD(x)
#endif


void fsp_scan_begin(const char *filename, int trace_scanning);
void fsp_scan_end();

/* ============================== FspDriver ============================= */

FspDriver::FspDriver()
{
    trace_scanning = trace_parsing = false;
    tree = NULL;
}

FspDriver::~FspDriver()
{
    this->clear();
}

void FspDriver::clear()
{
    if (remove_file.size()) {
	remove(remove_file.c_str());
	remove_file = "";
    }
    if (tree) {
        delete tree;
        tree = NULL;
    }
}

static bool isCompositeDefinition(fsp::ParametricProcess *pp)
{
    fsp::ProcessDefNode *pdn;
    fsp::CompositeDefNode *cdn;

    assert(pp && pp->translator);
    pdn = dynamic_cast<fsp::ProcessDefNode *>(pp->translator);
    cdn = dynamic_cast<fsp::CompositeDefNode *>(pp->translator);
    assert(cdn || pdn);

    return cdn != NULL;
}

bool FspDriver::shouldTranslateNow(const string& name)
{
    vector<string> dependencies;
    fsp::Symbol *svp;
    fsp::ParametricProcess *pp;

    if (!cop.shell) {
        /* Not in interactive mode? Translate everything. */
        return true;
    }

    /* When in interactive mode, defer composite processes translation. */
    if (!parametric_processes.lookup(name, svp)) {
        assert(0);
    }
    pp = fsp::is<fsp::ParametricProcess>(svp);

    if (isCompositeDefinition(pp)) {
        return false;
    }

    /* Also defer translation for those simple processes which depends
       at least on a composite process. */
    deps.findDependencies(name, dependencies);
    for (unsigned int i = 0; i < dependencies.size(); i++) {
        if (!parametric_processes.lookup(dependencies[i], svp)) {
            assert(0);
        }
        pp = fsp::is<fsp::ParametricProcess>(svp);
        if (isCompositeDefinition(pp)) {
            return false;
        }
    }

    return true;
}

void FspDriver::translateDeclarations()
{
    vector<string> classes;
    vector<fsp::TreeNode *> declarations;

    assert(tree);
    /* Find all the tree nodes that don't correspond to process
       definitions (e.g. declarations). Observe that a property
       definition is considered to be a process definition. */
    classes.push_back(fsp::ConstantDefNode::className());
    classes.push_back(fsp::RangeDefNode::className());
    classes.push_back(fsp::SetDefNode::className());
    classes.push_back(fsp::ProgressDefNode::className());
    classes.push_back(fsp::MenuDefNode::className());
    tree->getNodesByClasses(classes, declarations);

    /* Translate the declarations found. This results in filling
       in the following symbols tables: 'identifiers', 'progresses'
       and 'menus'. */
    for (unsigned int i = 0; i < declarations.size(); i++) {
        declarations[i]->translate(*this);
    }
}

void FspDriver::findParametricProcesses()
{
    vector<string> classes;
    vector<fsp::TreeNode *> definitions;

    assert(tree);
    /* Find all the tree nodes that correspond to process definitions
       (either simple or composite). */
    classes.push_back(fsp::ProcessDefNode::className());
    classes.push_back(fsp::CompositeDefNode::className());
    tree->getNodesByClasses(classes, definitions);

    for (unsigned int i = 0; i < definitions.size(); i++) {
        /* For each process definition, find all the parameters name
           and default values, building a ParametricProcess object.
           This object is then stored into the 'parametric_process'
           symbols table. */
        TDCS(fsp::ProcessDefNode, pdn, definitions[i]);
        TDCS(fsp::CompositeDefNode, cdn, definitions[i]);
        fsp::ProcessIdNode *pid;
        fsp::ParamNode *pan;
        fsp::ParametricProcess *pp = new fsp::ParametricProcess;

        /* Find the ProcessIdNode and ParamNode depending on the process
           definition type. */
        if (pdn) {
            pid = fsp::tree_downcast<fsp::ProcessIdNode>(pdn->getChild(1));
            pan = fsp::tree_downcast_null<fsp::ParamNode>(pdn->getChild(2));
            pp->set_translator(pdn);
        } else if (cdn) {
            pid = fsp::tree_downcast<fsp::ProcessIdNode>(cdn->getChild(1));
            pan = fsp::tree_downcast_null<fsp::ParamNode>(cdn->getChild(2));
            pp->set_translator(cdn);
        } else {
            assert(0);
        }

        /* Compute the name. */
        RDC(fsp::StringS, id, pid->translate(*this));

        if (pan) {
            /* Find the parameters. */
            TDC(fsp::ParameterListNode, pln, pan->getChild(1));

            for (unsigned int i = 0; i < pln->numChildren(); i += 2) {
                TDC(fsp::ParameterNode, p, pln->getChild(i));
                /* parameter_id = EXPR */
                RDC(fsp::StringS, paid,
                                p->getChild(0)->translate(*this));
                RDC(fsp::IntS, expr, p->getChild(2)->translate(*this));

                if (!pp->insert(paid->val, expr->val)) {
                    stringstream errstream;
                    errstream << "parameter " << paid->val <<
                                    " declared twice";
                    semantic_error(*this, errstream, p->getLocation());
                }
                delete paid;
                delete expr;
            }
        }

        /* Insert into the symbol table. */
        if (!this->parametric_processes.insert(id->val, pp)) {
            stringstream errstream;

            delete pp;
            errstream << "Parametric process " << id->val
                        << " already declared";
            semantic_error(*this, errstream, pid->getLocation());
        }

        delete id;
    }
}

/* Computes this->deps. */
void FspDriver::computeDependencyGraph()
{
    map<string, fsp::Symbol*>::iterator it;
    vector<string> classes;

    classes.push_back(fsp::ProcessRefSeqNode::className());
    classes.push_back(fsp::ProcessRefNode::className());

    /* Scan all the parametric processes table. */
    for (it = parametric_processes.table.begin();
                    it != parametric_processes.table.end(); it++) {
        fsp::ParametricProcess *pp = fsp::is<fsp::ParametricProcess>(it->second);
        vector<fsp::TreeNode *> references;
        fsp::TreeNode *ltn = dynamic_cast<fsp::LtsTreeNode* >(pp->translator);

        assert(ltn);
        /* Find all the (non local) processes referenced by this (non local)
           process. */
        ltn->getNodesByClasses(classes, references);
        for (unsigned int j = 0; j < references.size(); j++) {
            TDCS(fsp::ProcessRefSeqNode, seq, references[j]);
            TDCS(fsp::ProcessRefNode, ref, references[j]);

            assert(seq || ref);
            assert(references[j]->getChild(0));

            RDC(fsp::StringS, id,
                    references[j]->getChild(0)->translate(*this));

            /* Insert the dependency into the graph. */
            deps.add(it->first, id->val);

            delete id;
        }
    }

    IFD(deps.print());
}

void FspDriver::doProcessesTranslation()
{
    /* Do the translation. */
    for (map<string, fsp::Symbol*>::iterator it =
            parametric_processes.table.begin();
                it != parametric_processes.table.end(); it++) {
        fsp::ParametricProcess *pp = fsp::is<fsp::ParametricProcess>(it->second);
        fsp::LtsTreeNode *ltn;
        fsp::SmartPtr<fsp::Lts> lts;

        if (shouldTranslateNow(it->first)) {
            ltn = dynamic_cast<fsp::LtsTreeNode *>(pp->translator);
            assert(ltn);
            /* Translate the process definition, filling in the 'processes'
               symbol table. Note that we don't call 'ltn->translate(*this)'
               directly, but we use the 'process_ref_translate()' wrapper
               function (which is also used with process references).
               This function also setups and restore the translator context,
               taking care of the default process parameters and overridden
               identifiers.
               Note that the last argument is 'false', since we don't want
               a clone returned into '*lts' (we don't even need to access it).
            */
            process_ref_translate(*this, ltn->getLocation(), it->first,
                    NULL, &lts, false);
        }
    }
}

void FspDriver::translateProcessesDefinitions()
{
    /* These three functions must be called in this order. */
    findParametricProcesses();
    computeDependencyGraph();
    doProcessesTranslation();
}

/* First phase of the 'parse' method.
   At the end of this phase the symbols tables will contain all
   the objects specified (source FSP) or contained (binary objects) in
   the input file.
*/
int FspDriver::inputPhase(stringstream& ss)
{
    fsp::ActionsTable& actions = fsp::ActionsTable::getref();
    Deserializer *desp = NULL;
    int ret = 0;

    if (cop.input_type == CompilerOptions::InputTypeFsp) {
	string orig(cop.input_file);
	string temp = get_tmp_name_cwd(orig);

	/* Preprocess the input file, producing a temporary file. */
	ret = preprocess(orig, temp);
        if (ret) {
            return ret;
        }
	this->remove_file = temp;

	/* Parse the preprocessed temporary file. */
	fsp_scan_begin(temp.c_str(), trace_scanning);
	fsp::FspParser parser(*this);
	parser.set_debug_level(trace_parsing);
	ret = parser.parse();
	fsp_scan_end();

	/* Remove the temporary file. */
	remove(temp.c_str());
	this->remove_file = "";

        if (ret) {
            /* On error, the parser returns 1. */
            return ret;
        }

        assert(tree);
#if 0
        /* Output a GraphViz representation of the parse tree. */
        ofstream treef("tree.gv");
        tree->print(treef);
        treef.close();
#endif
        /* Translate the declarations (e.g. range, set, progress, ...). */
        translateDeclarations();

        /* Collect and translate the process definitions. */
        translateProcessesDefinitions();

        DBRT(fsp::PtrCheckTable::get()->check());
    } else { /* Load the processes table from an LTS file. */
	uint32_t nlts, nprogr;

	desp = new Deserializer(cop.input_file);

	desp->actions_table(actions, 0);
	desp->integer(nlts, 0);
	for (uint32_t i=0; i<nlts; i++) {
	    fsp::SmartPtr<fsp::Lts> lts =
                    new fsp::Lts(LtsNode::End);

	    desp->lts(*lts, 0);
	    /* Insert lts into the global 'processes' table. */
	    if (!processes.insert(lts->name, lts.delegate())) {
                /* This should never happen: Two LTSs are saved with the
                   same name into the compiled file. */
		assert(0);
	    }
	}

	desp->integer(nprogr, 0);
	for (uint32_t i=0; i<nprogr; i++) {
	    fsp::ProgressS *pv = new fsp::ProgressS;
	    string name;

	    desp->stl_string(name, 0);
	    desp->progress_value(*pv, 0);
	    if (!progresses.insert(name, pv)) {
		assert(0);
	    }
	}
    }

    if (desp)
	delete desp;

    return 0;
}

int FspDriver::outputPhase(stringstream& ss)
{
    fsp::ActionsTable& actions = fsp::ActionsTable::getref();
    Serializer *serp = NULL;
    map<string, fsp::Symbol *>::iterator it;
    map<string, fsp::Symbol *>::iterator jt;
    fsp::SmartPtr<fsp::Lts> lts;
    fsp::ProgressS *pv;

    if (cop.input_type == CompilerOptions::InputTypeFsp && cop.output_file) {
        serp = new Serializer(cop.output_file);
    }

    if (serp) {
	serp->actions_table(actions, 0);
	serp->integer(processes.table.size(), 0);
    }

    /* Scan the 'processes' symbols table. For each process, output
       the associated LTS and do the deadlock analysis. */
    for (it=processes.table.begin(); it!=processes.table.end(); it++) {
	lts = fsp::is<fsp::Lts>(it->second);

	/* We output an LTS file only if the input is not an LTS file. */
	if (serp) {
	    serp->lts(*lts, 0);
	}

	if (cop.deadlock) {
	    lts->deadlockAnalysis(ss);
	}

	if (cop.graphviz) {
	    lts->graphvizOutput((lts->name + ".gv").c_str(), true);
	}
    }

    if (serp) {
	serp->integer(progresses.table.size(), 0);
    }
    /* Do each progress check against all the global processes. */
    for (it=progresses.table.begin(); it!=progresses.table.end();
	    it++) {
	pv = fsp::is<fsp::ProgressS>(it->second);
	if (cop.progress) {
	    for (jt=processes.table.begin();
		    jt!=processes.table.end(); jt++) {
		lts = fsp::is<fsp::Lts>(jt->second);
		lts->progress(it->first, *pv, ss);
	    }
	}

	/* Output the property if the input is not an LTS file. */
	if (serp) {
	    serp->stl_string(it->first, 0);
	    serp->progress_value(*pv, 0);
	}
    }

    if (serp)
	delete serp;

    return 0;
}

int FspDriver::parse(const CompilerOptions& co)
{
    stringstream ss;
    int ret = 0;

    /* Copy in the options. */
    cop = co;

    ret = inputPhase(ss);
    if (ret) {
        return ret;
    }

    /* Run an LTS analysis script if the user asked for that. */
    if (cop.script) {
	ifstream fin(cop.script_file, ios::in);

	if (fin.fail()) {
	    cerr << cop.script_file << ": no such script file\n";
	    exit(-1);
	}

	ret = BatchShell(*this, fin).run();
	fin.close();
    }

    /* Run the interactive shell if the user asked for that. */
    if (cop.shell) {
	ret = InteractiveShell(*this, std::cin).run();
    }

    outputPhase(ss);

    /* Flush out program output. */
    cout << ss.str();

    return ret;
}

bool FspDriver::nesting_save()
{
    if (nesting_stack.size() > cop.max_reference_depth) {
        return false;
    }

    /* Save the current context and push it on the
       nesting stack. */
    nesting_stack.push_back(NestingContext());
    nesting_stack.back().ctx = ctx;
    nesting_stack.back().unres = unres;
    nesting_stack.back().parameters = parameters;
    nesting_stack.back().overridden_names = overridden_names;
    nesting_stack.back().overridden_values = overridden_values;

    /* Clean the current context. */
    ctx.clear();
    unres.clear();
    parameters.clear();
    overridden_names.clear();
    overridden_values.clear();

    return true;
}

void FspDriver::nesting_restore()
{
    /* Remove the parameters identifiers that have been used
       during the last process translation. */
    for (unsigned int i=0; i<parameters.names.size(); i++) {
        identifiers.remove(parameters.names[i]);
    }
    /* Restore overridden identifiers. */
    for (unsigned int i=0; i<overridden_names.size(); i++) {
        if (!identifiers.insert(overridden_names[i],
                    overridden_values[i])) {
            assert(0);
            delete overridden_values[i];
        }
    }

    /* Pop the last saved context. */
    assert(nesting_stack.size());
    ctx = nesting_stack.back().ctx;
    unres = nesting_stack.back().unres;
    parameters = nesting_stack.back().parameters;
    overridden_names = nesting_stack.back().overridden_names;
    overridden_values = nesting_stack.back().overridden_values;
    nesting_stack.pop_back();
}

static bool parse_extended_name(const string& name, string& base,
                                vector<int>& args)
{
    size_t ope;

    args.clear();

    ope = name.find_first_of('(');
    if (ope == string::npos) {
        /* No parameters specified. */
        if (name.find_first_of(')') != string::npos) {
            return false;
        }
        base = name;
    } else {
        size_t clo;

        /* Some parameters have been specified. */
        if (ope == 0) {
            return false;
        }
        if (name.find_last_of('(') != ope) {
            /* More than one '('. */
            return false;
        }
        clo = name.find_first_of(')');
        if (clo == string::npos) {
            return false;
        }
        if (name.find_last_of(')') != clo) {
            /* More than one ')'. */
            return false;
        }
        if (ope >= clo) {
            return false;
        }
        /* Space outside the () are not allowed. */
        for (unsigned int i = 0; i < ope; i++) {
            if (name[i] == ' ' || name[i] == '\t') {
                return false;
            }
        }
        for (unsigned int i = clo+1; i < name.size(); i++) {
            if (name[i] != ' ' && name[i] != '\t') {
                return false;
            }
        }

        /* Extract the base name. */
        base = name.substr(0, ope);

        /* Extract the parameters. */
        size_t a = ope + 1;

        for (;;) {
            size_t b = name.find_first_of(',', a);
            int x;

            if (b == string::npos) {
                b = clo;
            }
            if (string2int(name.substr(a, b - a), x)) {
                return false;
            }
            args.push_back(x);

            if (b == clo) {
                break;
            }
            a = b + 1;
        }
    }

    return true;
}

/* Ask the compiler for the LTS corresponding to the FSP reference
   specified by 'name'.
   If 'create' is 'true', the compiler can perform a translation, when
   needed. If 'create' is 'false', the compiler doesn't perform
   any translations, but only looks up the 'processes' cache.
   If the return pointer is not NULL, the pointed Lts instance is the
   the same contained in the 'processes' table: It is not a clone.
*/
fsp::SmartPtr<fsp::Lts> FspDriver::getLts(const string& name, bool create)
{
    fsp::Symbol *svp;
    fsp::ParametricProcess *pp;
    fsp::SmartPtr<fsp::Lts> lts;
    fsp::LtsTreeNode *ltn;
    string base;
    vector<int> args;
    bool have_parametric_processes = parametric_processes.size();

    /* The user-specified string 'name' shuould be in the form
        'FSPNAME(VAL1,VAL2,VAL3,...)'. This helper function extracts
       'FSPNAME' into base and extracts the integers VAL1, VAL2, ... into
       'args'.
    */
    if (!parse_extended_name(name, base, args)) {
        /* The user string is not valid. */
        return NULL;
    }

    /* If the fspcc input is an FSP file, then the 'parametric_processes'
       table (usually) contains some elements, and it is therefore possible
       to perform translations (it makes sense talking about translations).
    */
    if (have_parametric_processes) {
        if (!parametric_processes.lookup(base, svp)) {
            /* String valid, but the user just made up a process name. */
            return NULL;
        }
        pp = fsp::is<fsp::ParametricProcess>(svp);

        /* If 'name' specifies some parameters, their number must match
           the requested number of parameters. We do this check here to
           avoid a semantic_error. */
        if (args.size() && args.size() != pp->defaults.size()) {
            return NULL;
        }
        /* If 'name' doesn't specify parameters, use defaults. */
        if (!args.size()) {
            args = pp->defaults;
        }

        if (create) {
            /* Everything is ok and we can translate, if necessary:
               Take the translator and use it. We don't ask for a clone,
               (see the description of this method). */
            ltn = dynamic_cast<fsp::LtsTreeNode *>(pp->translator);
            assert(ltn);
            process_ref_translate(*this, ltn->getLocation(), base,
                    &args, &lts, false);
        }
    }

    if (!have_parametric_processes || !create) {
        /* Everything is ok, but we don't want to force the translation if
           it has not already been done, either because the user doesn't
           want or because 'parametric_processes' is empty. */
        fsp::Symbol *svp;
        string extension;

        lts_name_extension(args, extension);
        if (!processes.lookup(base + extension, svp)) {
            return NULL;
        }
        return fsp::is<fsp::Lts>(svp);
    }

    return lts;
}

void FspDriver::error(const fsp::location& l, const std::string& m)
{
    print_error_location_pretty(l);
    cerr << m << endl;
}

void FspDriver::error(const std::string& m)
{
    cerr << m << endl;
}


/* ======================= DependencyGraph ======================= */

bool DependencyGraph::add(const string& depends, const string& on)
{
    table_iterator mit;

    mit = table.find(depends);
    if (mit == table.end()) {
        pair<table_iterator, bool> ret;

        ret = table.insert(make_pair(depends, vector<string>()));
        assert(ret.second);
        mit = ret.first;
    }

    /* Check against duplicates. */
    for (unsigned int i = 0; i < mit->second.size(); i++) {
        if (on == mit->second[i]) {
            return false;
        }
    }
    mit->second.push_back(on);

    return true;
}

void DependencyGraph::findDependencies(const string& depends,
                                       vector<string>& result)
{
    /* BFS on the dependency graph. The graph is __almost__ acyclic,
       that is is acyclic if we don't consider recursive dependencies. */
    queue<string> frontier;

    result.clear();
    frontier.push(depends);

    while (!frontier.empty()) {
        const string& id = frontier.front();
        table_iterator mit;

        result.push_back(id);
        mit = table.find(id);
        if (mit != table.end()) {
            for (unsigned int i = 0; i < mit->second.size(); i++) {
                /* This check is against recursive processes, which depends
                   on themselves, in order to avoid entering a
                   non-terminating loop. */
                if (mit->second[i] != id) {
                    frontier.push(mit->second[i]);
                }
            }
        }
        frontier.pop();
    }

    /* Reverse the order of the dependencies found, so that the caller can
       translate in order and always hit the 'processes' cache. */
    for (unsigned int i = 0; i < result.size()/2; i++) {
        string tmp = result[i];

        result[i] = result[result.size() - 1 - i];
        result[result.size() - 1 - i] = tmp;
    }
}

void DependencyGraph::print()
{
    for (table_iterator mit = table.begin(); mit != table.end(); mit++) {
        cout << mit->first << " --> ";
        for (unsigned int i = 0; i < mit->second.size(); i++) {
            cout << mit->second[i] << " ";
        }
        cout << "\n";
    }
}

