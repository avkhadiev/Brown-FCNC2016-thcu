#!/bin/bash

# global variables
SOURCELFN="/store/user/snowmass/noreplica/DelphesFromLHE_333pre16_2016Aug"
TARGETLFN="/store/group/upgrade/delphes_output/DelphesFromLHE_333pre16_2016Aug"
SOURCEMGM="root://cmseos.fnal.gov"                                               
TARGETMGM="root://eoscms.cern.ch"                                               
FILEMASK="*.root"
dirnames=(
    "TT_antitopHadronicDecay_eta_hct_LO_14TEV_0PU"
    "TT_antitopHadronicDecay_eta_hct_LO_14TEV_200PU"
    "TT_antitopHadronicDecay_eta_hut_LO_14TEV_0PU"
    "TT_antitopHadronicDecay_eta_hut_LO_14TEV_200PU"
    "TT_antitopLeptonicDecay_eta_hct_LO_14TEV_0PU"
    "TT_antitopLeptonicDecay_eta_hct_LO_14TEV_200PU"
    "TT_antitopLeptonicDecay_eta_hut_LO_14TEV_0PU"
    "TT_antitopLeptonicDecay_eta_hut_LO_14TEV_200PU"
    "TT_topHadronicDecay_eta_hct_LO_14TEV_0PU"
    "TT_topHadronicDecay_eta_hct_LO_14TEV_200PU"
    "TT_topHadronicDecay_eta_hut_LO_14TEV_0PU"
    "TT_topHadronicDecay_eta_hut_LO_14TEV_200PU"
    "TT_topLeptonicDecay_eta_hct_LO_14TEV_0PU"
    "TT_topLeptonicDecay_eta_hct_LO_14TEV_200PU"
    "TT_topLeptonicDecay_eta_hut_LO_14TEV_0PU"
    "TT_topLeptonicDecay_eta_hut_LO_14TEV_200PU"
    )

#################################################################################

# this function will copy files 
#   - from the URL composed by $SOURCEMGM and $SOURCELFN 
#   - to the URL composed by $TARGETMGM and $TARGETLFN. 
#   - it  only copies the files from subdirectories specified in $subdirs. 
#   - it copies ALL files in these subdirectories.
#   - in target, it will create subdirs with the same names, unless they already exist.

copy () {
    echo "Initializing the CMS environment..."
    cd /uscms_data/d3/aavkhadi/FCNC/CMSSW_8_0_11/src/                               # initialize CMSSW environment
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    eval scramv1 runtime -sh
    echo "Environment initialized"
    cd /uscms_data/d3/aavkhadi/FCNC/CMSSW_8_0_11/src/Brown-FCNC2016-thcu/SAMPLES    # directory where script is executed

    echo "Assuming your VOMS proxy is initialized"
    for dir in ${dirnames[*]} ; do 
         echo "Creating a directory $dir"
        xrdfs $TARGETMGM mkdir -p $TARGETLFN/${dir}
        # loop over directories in the source
        for filepath in $(xrdfs $SOURCEMGM ls ${SOURCELFN}/${dir}) ; do
            # copy the files
            echo "Will copy ALL files from the directory ${dir}"
            xrdcp ${SOURCEMGM}/${filepath} ${TARGETMGM}/${TARGETLFN}/${dir}/
        done 
    done
}

clear
echo "Will copy files:"
echo "    from the source LFN: ${SOURCELFN}"
echo "    to the target LFN: ${TARGETLFN}"
echo "Using the following MGMs (redirectors):"
echo "    source MGM ${SOURCEMGM}"
echo "    target MGM ${TARGETMGM}"
echo
echo "In the source, will look for the following directories:"
for dir in ${dirnames[*]} ; do 
    echo ${dir}
done
echo
echo "Will create the same directories in the target, unless they already exist."
echo "Will use xrdcp to copy ALL files in these directories. Make sure they don't already exist in the target: xrdcp will not overwrite, and error will be thrown."
echo "Make sure your have initialized your VOMS proxy before running this script."
echo "Checking your VOMS proxy initialization. No output means your VOMS is initialized..."
voms-proxy-info --exists
echo
while true; do
    read -p "Do you wish to continue (y or n)?" yn
    case $yn in
        [Yy]* ) copy; break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no.";;
    esac
done
