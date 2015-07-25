#!/bin/bash
function apply_patch() {

patch -d JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1 -p1 << 'PATCH_STRING'
diff -U 1 -u -r JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/FWDistribution/Build/Makefile JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/FWDistribution/Build/Makefile
--- JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/FWDistribution/Build/Makefile	2013-01-10 09:43:55.000000000 +0000
+++ JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/FWDistribution/Build/Makefile	2015-07-25 02:48:00.371901159 +0000
@@ -34,7 +34,7 @@
 
 PROGRAM_VERSION := 0.11
 
-CFLAGS += -O2 -Wall -g
+CFLAGS += -O2 -Wall -g -I/usr/include/lua5.1
 
 
 
diff -U 1 -u -r JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/GatewayFactoryTest/Build/Makefile JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/GatewayFactoryTest/Build/Makefile
--- JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/GatewayFactoryTest/Build/Makefile	2012-06-18 14:31:24.000000000 +0000
+++ JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/GatewayFactoryTest/Build/Makefile	2015-07-25 03:21:58.111792325 +0000
@@ -64,8 +64,8 @@
 	$(CC)  -I. $(CFLAGS) $(PROJ_CFLAGS) -c $<
 
 install:
-	mkdir -p $(DESTDIR)/usr/bin/
-	cp $(TARGET) $(DESTDIR)/usr/bin/
+	mkdir -p $(DESTDIR)/bin/
+	cp $(TARGET) $(DESTDIR)/bin/
 
 clean:
 	rm -f *.o $(TARGET) FTProg
diff -U 1 -u -r JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/JennicModuleProgrammer/Build/Makefile JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/JennicModuleProgrammer/Build/Makefile
--- JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/JennicModuleProgrammer/Build/Makefile	2013-06-24 08:41:04.000000000 +0000
+++ JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/JennicModuleProgrammer/Build/Makefile	2015-07-25 03:13:13.109819264 +0000
@@ -56,8 +56,8 @@
 	$(CC)  -I. $(CFLAGS) $(PROJ_CFLAGS) -c $<
 
 install:
-	mkdir -p $(DESTDIR)/usr/bin/
-	cp $(TARGET) $(DESTDIR)/usr/bin/
+	mkdir -p $(DESTDIR)/bin/
+	cp $(TARGET) $(DESTDIR)/bin/
 
 clean:
 	rm -f *.o $(TARGET)
diff -U 1 -u -r JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/JIPd/Build/Makefile JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/JIPd/Build/Makefile
--- JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/JIPd/Build/Makefile	2014-07-29 09:42:16.000000000 +0000
+++ JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/JIPd/Build/Makefile	2015-07-25 03:17:05.661833355 +0000
@@ -31,6 +31,8 @@
 
 TARGET_JIPD   = JIPd
 
+INSTALL ?= install
+
 ##############################################################################
 # Default target is the JN514x family since we're building a library
 
@@ -150,6 +152,9 @@
 	$(info Linking $@ ...)
 	$(CC) -o $@ $^ $(LDFLAGS) $(PROJ_LDFLAGS)
 
+install:
+	$(INSTALL) $(TARGET_JIPD) $(DESTDIR)/sbin
+
 clean:
 	rm -f *.o
 	rm -f *.d
diff -U 1 -u -r JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/libJIP/Build/Makefile JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/libJIP/Build/Makefile
--- JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/libJIP/Build/Makefile	2014-07-29 09:20:50.000000000 +0000
+++ JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/libJIP/Build/Makefile	2015-07-25 03:08:15.100061500 +0000
@@ -179,9 +179,9 @@
 	rm -f $(TARGET_CLI) $(JIPCLIOBJS)
 
 install:
-	$(INSTALL) $(LIBJIP_INC)/JIP.h $(DESTDIR)/usr/include
-	$(INSTALL) $(LIBJIP_LIB)/* $(DESTDIR)/usr/lib/
-	$(INSTALL) $(TARGET_CLI) $(DESTDIR)/usr/bin/
+	$(INSTALL) $(LIBJIP_INC)/JIP.h $(DESTDIR)/include
+	$(INSTALL) $(LIBJIP_LIB)/* $(DESTDIR)/lib/
+	$(INSTALL) $(TARGET_CLI) $(DESTDIR)/bin/
 
 uninstall:
 	rm $(DESTDIR)/usr/include/JIP.h
diff -U 1 -u -r JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/zigbee-jip-daemon/JIP/Build/Makefile JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/zigbee-jip-daemon/JIP/Build/Makefile
--- JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/zigbee-jip-daemon/JIP/Build/Makefile	2014-05-14 08:59:01.000000000 +0000
+++ JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/zigbee-jip-daemon/JIP/Build/Makefile	2015-07-25 02:48:00.381900815 +0000
@@ -44,8 +44,10 @@
 
 PROJ_CFLAGS += -I../../ZCB/Include/
 
-PROJ_CFLAGS += -I../../../../JIP/libJIP/Include/
-PROJ_LDFLAGS += -L../../../../JIP/libJIP/Library -lJIP
+#PROJ_CFLAGS += -I../../../../JIP/libJIP/Include/
+#PROJ_LDFLAGS += -L../../../../JIP/libJIP/Library -lJIP
+PROJ_CFLAGS += -I../../../libJIP/Include/
+PROJ_LDFLAGS += -L../../../libJIP/Library -lJIP
 
 PROJ_LDFLAGS += -ldaemon -lpthread -lJIP -lsqlite3 -lxml2
 
diff -U 1 -u -r JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/zigbee-jip-daemon/ZCB/Build/Makefile JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/zigbee-jip-daemon/ZCB/Build/Makefile
--- JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1.orig/Host/Source/zigbee-jip-daemon/ZCB/Build/Makefile	2014-06-05 13:58:07.000000000 +0000
+++ JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source/zigbee-jip-daemon/ZCB/Build/Makefile	2015-07-25 02:48:00.381900815 +0000
@@ -40,8 +40,10 @@
 
 PROJ_CFLAGS += -I../Include/
 
-PROJ_CFLAGS += -I../../../../JIP/libJIP/Include/
-PROJ_LDFLAGS += -L../../../../JIP/libJIP/Library -lJIP
+#PROJ_CFLAGS += -I../../../../JIP/libJIP/Include/
+#PROJ_LDFLAGS += -L../../../../JIP/libJIP/Library -lJIP
+PROJ_CFLAGS += -I../../../libJIP/Include/
+PROJ_LDFLAGS += -L../../../libJIP/Library -lJIP
 
 
 vpath %.c ../Source
PATCH_STRING

}

##########################################################################################################################################

SRCDIR=JN-AN-1110_JenNet-IP-Border-Router-BR_1v9v3_RC1/Host/Source

DIRS=( "6LoWPANd" "FWDistribution" "JennicModuleProgrammer" "GatewayFactoryTest" "libJIP" "JIPd" "JIPweb" "NFCd" "zigbee-jip-daemon/JIP" )

if [ ! -v DESTDIR ]
then
	DESTDIR=/usr/local
fi

if [ "$1" = "install" ]
then
	for DIR in ${DIRS[@]}
	do
		make -C ${SRCDIR}/${DIR}/Build install DESTDIR=${DESTDIR}
	done
	exit 0
fi

if [ "$1" = "clean" ]
then
	rm -rf  ${SRCDIR}/
	exit 0
fi

if [ "$1" = "download" ]
then
	wget http://www.nxp.com/documents/other/JN-AN-1110.zip
fi

if [ "$1" = "extract" ]
then
	unzip -o JN-AN-1110.zip "${SRCDIR}/*"
	for TGZ in $(find `pwd`/${SRCDIR}/ -name *.tar.gz ) ; do ( cd `dirname $TGZ` ; tar xvfz $TGZ ) ; done 

	ln -s ${SRCDIR}/zigbee-jip-daemon/Build/version.txt ${SRCDIR}/zigbee-jip-daemon/JIP/Build/
	ln -s ${SRCDIR}/zigbee-jip-daemon/Build/version.txt ${SRCDIR}/zigbee-jip-daemon/ZCB/Build/

	apply_patch

	exit 0
fi

if [ "$1" = "apt-install" ]
then
	sudo apt-get install libavahi-client-dev liblua5.1-0-dev libftdi-dev libjson-c-dev libtecla-dev libsqlite3-dev
	exit 0
fi



for DIR in ${DIRS[@]}
do
	make -C ${SRCDIR}/${DIR}/Build
done

