all: classes run manifest

run: $(classes)
	touch run.sh
	chmod 754 run.sh
	echo "#!/bin/bash" > run.sh
	echo "java -jar q3.jar" >> run.sh

classes:
	javac *.java

manifest: $(classes)
	echo "Manifest-Version: 1.0" > Manifest.txt
	echo "Class-Path: ." >> Manifest.txt
	echo "Main-Class: Main" >> Manifest.txt 
	jar cfm q3.jar Manifest.txt *.class


clean:
	rm *.class
	rm *.jar
	rm Manifest.txt