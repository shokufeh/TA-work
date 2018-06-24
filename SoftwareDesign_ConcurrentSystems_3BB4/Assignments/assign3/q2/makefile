all: classes run manifest

classes:
	javac *.java

manifest: $(classes)
	echo "Manifest-Version: 1.0" > Manifest.txt
	echo "Class-Path: ." >> Manifest.txt
	echo "Main-Class: Bank" >> Manifest.txt
	jar cfm Bank.jar Manifest.txt Bank.class

run:
	echo #!bin/bash>run
	echo java -jar Bank.jar >> run
	chmod 754 run
clean:
	rm *.class
	rm run
	rm Manifest.txt
	rm *.jar
