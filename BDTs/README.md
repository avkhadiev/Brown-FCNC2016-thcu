# Implementing a binary classifier for FCNC events
The current implementation is boosted decision trees ([BDTs](https://homes.cs.washington.edu/~tqchen/data/pdf/BoostedTree.pdf "Introducton to Boosted Decison Trees")) method from ROOT's [TMVA package](http://tmva.sourceforge.net/ "TMVA on SourceForge"). Consult the Users' Guide (in .pdf) [here](http://tmva.sourceforge.net/docu/TMVAUsersGuide.pdf "TMVA Users' Guide").

## What to do
- Create configuration files based on [sample configs][1] from TMVA and those in [Nhan's repository][2].
- Implement infrastructure for classifier training [condorization][3] (on LPC), in order to optimize the BDTs. 

[1]: https://sourceforge.net/p/tmva/code/HEAD/tree/trunk/TMVA/macros/
[2]: https://github.com/nhanvtran/trackObservables/tree/master/analysis
[3]: http://uscms.org/uscms_at_work/physics/computing/setup/batch_systems.shtml#condor_1

## Notes on implementation
- Will run lots of trainings, tests, and evaluations with different configurations, where reconfiguration consists of changing input variables, spectator variables, weights, and selection cuts.
- Will display, store, and re-display the results of trainings, tests, and evaluations convieniently
- Will condorize jobs for training, test, and evaluation, staging out the results on LPC. 

Therefore:
- Need to create easy-to-configure containers to store all the parameters for TMVA training, testing, and evaluation. These containers will have designated data members to store: trees, variables, spectators, cuts, MVA methods, and weights; they will also have all the methods for re-configuration: adding, displaying, and deleting their data members. 
- Need to create a configurer class to set up a TMVA::Factory according to the settings in any given instance of a container. This configurer will take in a given container with all the settings, and utilize the TMVA Factory to train, test, and evaluate.
- Need to write a script that will set up many different containers and then use the configurer with all the containers, submitting the jobs via Condor. 
