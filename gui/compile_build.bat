@echo off
javac GUIInterface.java
jar cvmf manifest.txt GUIInterface.jar GUIInterface.class GUIInterface$1.class GUIInterface$2.class GUIInterface$3.class GUIInterface$4.class GUIInterface$5.class
java -jar GUIInterface.jar Main
pause