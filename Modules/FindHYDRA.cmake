# - Find HYDRA instalation
# This module tries to find the HYDRA installation on your system.
# It tries to find the root-config script which gives you all the needed information.
# If the system variable ROOTSYS is set this is straight forward.
# If not the module uses the pathes given in HYDRA_CONFIG_SEARCHPATH.
# If you need an other path you should add this path to this varaible.  
# The root-config script is then used to detect basically everything else.
# This module defines a number of key variables and macros.

# F.Uhlig@gsi.de (fairroot.gsi.de)

# Oryginal module written for ROOT, adapted for HYDRA by
# Rafal Lalik ( Rafal.Lalik@ph.tum.de )


MESSAGE(STATUS "Looking for Hydra...")

IF (NOT HADDIR)
	SET (INTHADDIR $ENV{HADDIR})
ENDIF (NOT HADDIR)  

IF (NOT INTHADDIR)
  MESSAGE( FATAL_ERROR "HADDIR is not set. Please set HADDIR first.")
ENDIF (NOT INTHADDIR)

SET(HYDRA_PACKAGE_SEARCHPATH
	${INTHADDIR}/lib
)

SET(HYDRA_DEFINITIONS "")

SET(HYDRA_INSTALLED_VERSION_TOO_OLD FALSE)

SET(HYDRA_MAIN_LIBRARY HYDRA_MAIN_LIBRARY-NOTFOUND)

FIND_LIBRARY(HYDRA_MAIN_LIBRARY NAMES Hydra PATHS
	${HYDRA_PACKAGE_SEARCHPATH}
	NO_DEFAULT_PATH)
    
IF (${HYDRA_MAIN_LIBRARY} MATCHES "HYDRA_MAIN_LIBRARY-NOTFOUND")
	MESSAGE( FATAL_ERROR "Hydra library not found. Please check your Hydra installation.")
ELSE (${HYDRA_MAIN_LIBRARY} MATCHES "HYDRA_MAIN_LIBRARY-NOTFOUND")
	MESSAGE(STATUS "Looking for Hydra... - found ${INTHADDIR}")
ENDIF (${HYDRA_MAIN_LIBRARY} MATCHES "HYDRA_MAIN_LIBRARY-NOTFOUND")  

SET(HYDRA_FOUND TRUE)

IF (HYDRA_FOUND)
	SET(HYDRA_LIBRARY_DIR ${INTHADDIR}/lib )
	SET(HYDRA_INCLUDE_DIR ${INTHADDIR}/include )

	SET(HYDRA_LIBRARIES "")

	# Make variables changeble to the advanced user
	MARK_AS_ADVANCED( HYDRA_LIBRARY_DIR HYDRA_INCLUDE_DIR HYDRA_DEFINITIONS)

	# Set HYDRA_INCLUDES
	SET(HYDRA_INCLUDES ${HYDRA_INCLUDE_DIR})

	SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${HYDRA_LIBRARY_DIR})
ENDIF (HYDRA_FOUND)