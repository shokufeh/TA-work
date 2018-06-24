import subprocess
import sys


local = sys.argv[1] == 'local'
ver = sys.argv[2]
generating = False

# fields values
pkgname = 'fspcc'
pkgver = ver
src = "/release"
archive = pkgname + '-' + pkgver + '.tar.gz'
if local:
    url = 'http://127.0.0.1/$pkgname-$pkgver.tar.gz'
else:
    url = "https://bitbucket.org/vmaffione/fspc/downloads/fspcc-%s.tar.gz" % (ver, )

# output of the PKGBUILD
f = open("PKGBUILD", "w")

f.write('# Maintainer: Vincenzo Maffione <v.maffione@gmail.com>\n');
f.write('pkgname=%s\n' % (pkgname, ));
f.write('pkgver=%s\n' % (pkgver, ));
f.write('pkgrel=1\n');
f.write('pkgdesc="An FSP compiler and LTS analysis tool"\n');
f.write('arch=(\'i686\' \'x86_64\')\n');
f.write('url="%s"\n' % (url, ));
f.write('license=(\'GPL3\')\n');
f.write('depends=(\'bash\')\n');
if generating:
    f.write('makedepends=(\'bison\' \'flex\' \'sed\' \'python\')\n');
f.write('checkdepends=()\n');
f.write('optdepends=(\'graphviz: visualizing compiled LTS\' \'xv: visualizing compiled LTS\')\n');
f.write('source=("%s")\n' % (url, ));

#compute md5 checksum
ret = subprocess.check_output(["md5sum", archive])
f.write('md5sums=(\'%s\')\n' % ( ret[0:32].decode('latin-1'), ))

f.write('\nbuild() {\n'
            '\tcd "$srcdir%s"\n'
            '\tmake OPTIMIZE=-O2 || return 1 \n'
        '}\n' % (src, ));

f.write('\ncheck() {\n'
            '\techo "nothing to check"\n'
        '}\n');

f.write('\npackage() {\n'
            '\tcd "$srcdir%s"\n'
            '\tmkdir -p "$pkgdir/usr/bin"\n'
            '\tcp fspcc "$pkgdir/usr/bin"\n'
            '\tcp ltsee "$pkgdir/usr/bin"\n'
            '\tmkdir -p "$pkgdir/usr/share/man/man1"\n'
            '\tcp fspcc.1 "$pkgdir/usr/share/man/man1"\n'
            '\tgzip "$pkgdir/usr/share/man/man1/fspcc.1"\n'
        '}\n' % (src, ));

f.write('# vim:set ts=2 sw=2 et:\n')
f.close()
