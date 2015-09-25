#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/583062117/FileArchiver.o \
	${OBJECTDIR}/_ext/583062117/fileRec.o \
	${OBJECTDIR}/_ext/583062117/fileRecTestRun.o \
	${OBJECTDIR}/_ext/583062117/helperFuncs.o \
	${OBJECTDIR}/_ext/583062117/versionRec.o \
	${OBJECTDIR}/main.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/filearchivertest

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/filearchivertest: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/filearchivertest ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/583062117/FileArchiver.o: ../FileArchiver/FileArchiver.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/583062117
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/583062117/FileArchiver.o ../FileArchiver/FileArchiver.cpp

${OBJECTDIR}/_ext/583062117/fileRec.o: ../FileArchiver/fileRec.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/583062117
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/583062117/fileRec.o ../FileArchiver/fileRec.cpp

${OBJECTDIR}/_ext/583062117/fileRecTestRun.o: ../FileArchiver/fileRecTestRun.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/583062117
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/583062117/fileRecTestRun.o ../FileArchiver/fileRecTestRun.cpp

${OBJECTDIR}/_ext/583062117/helperFuncs.o: ../FileArchiver/helperFuncs.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/583062117
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/583062117/helperFuncs.o ../FileArchiver/helperFuncs.cpp

${OBJECTDIR}/_ext/583062117/versionRec.o: ../FileArchiver/versionRec.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/583062117
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/583062117/versionRec.o ../FileArchiver/versionRec.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/newtestclass.o ${TESTDIR}/tests/newtestrunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   


${TESTDIR}/tests/newtestclass.o: tests/newtestclass.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/newtestclass.o tests/newtestclass.cpp


${TESTDIR}/tests/newtestrunner.o: tests/newtestrunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/newtestrunner.o tests/newtestrunner.cpp


${OBJECTDIR}/_ext/583062117/FileArchiver_nomain.o: ${OBJECTDIR}/_ext/583062117/FileArchiver.o ../FileArchiver/FileArchiver.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/583062117
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/583062117/FileArchiver.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/583062117/FileArchiver_nomain.o ../FileArchiver/FileArchiver.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/583062117/FileArchiver.o ${OBJECTDIR}/_ext/583062117/FileArchiver_nomain.o;\
	fi

${OBJECTDIR}/_ext/583062117/fileRec_nomain.o: ${OBJECTDIR}/_ext/583062117/fileRec.o ../FileArchiver/fileRec.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/583062117
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/583062117/fileRec.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/583062117/fileRec_nomain.o ../FileArchiver/fileRec.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/583062117/fileRec.o ${OBJECTDIR}/_ext/583062117/fileRec_nomain.o;\
	fi

${OBJECTDIR}/_ext/583062117/fileRecTestRun_nomain.o: ${OBJECTDIR}/_ext/583062117/fileRecTestRun.o ../FileArchiver/fileRecTestRun.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/583062117
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/583062117/fileRecTestRun.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/583062117/fileRecTestRun_nomain.o ../FileArchiver/fileRecTestRun.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/583062117/fileRecTestRun.o ${OBJECTDIR}/_ext/583062117/fileRecTestRun_nomain.o;\
	fi

${OBJECTDIR}/_ext/583062117/helperFuncs_nomain.o: ${OBJECTDIR}/_ext/583062117/helperFuncs.o ../FileArchiver/helperFuncs.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/583062117
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/583062117/helperFuncs.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/583062117/helperFuncs_nomain.o ../FileArchiver/helperFuncs.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/583062117/helperFuncs.o ${OBJECTDIR}/_ext/583062117/helperFuncs_nomain.o;\
	fi

${OBJECTDIR}/_ext/583062117/versionRec_nomain.o: ${OBJECTDIR}/_ext/583062117/versionRec.o ../FileArchiver/versionRec.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/583062117
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/583062117/versionRec.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/583062117/versionRec_nomain.o ../FileArchiver/versionRec.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/583062117/versionRec.o ${OBJECTDIR}/_ext/583062117/versionRec_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/filearchivertest

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
