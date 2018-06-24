all: classes run manifest

classes:
	javac *.java

manifest: $(classes)
	echo "Manifest-Version: 1.0" > Manifest.txt
	echo "Class-Path: ." >> Manifest.txt
	echo "Main-Class: BankBroken" >> Manifest.txt
	jar cfm BankBroken.jar Manifest.txt BankBroken.class

run:
	echo #!bin/bash>run
	echo java -jar BankBroken.jar >> run
	chmod 754 run
clean:
	rm *.class
	rm run
	rm Manifest.txt
	rm *.jar
