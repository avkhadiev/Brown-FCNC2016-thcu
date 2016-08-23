# Implementing a binary classifier for FCNC events
The current implementation is boosted decision trees ([BDTs](https://homes.cs.washington.edu/~tqchen/data/pdf/BoostedTree.pdf "Introducton to Boosted Decison Trees")) method from ROOT's [TMVA package](http://tmva.sourceforge.net/ "TMVA on SourceForge"). Consult the Users' Guide (in .pdf) [here](http://tmva.sourceforge.net/docu/TMVAUsersGuide.pdf "TMVA Users' Guide").

## What to do
- Create configuration files based on [sample configs][1] from TMVA and those in [Nhan's repository][2].
- Implement infrastructure for classifier training [condorization][3] (on LPC), in order to optimize the BDTs. 

[1]: https://sourceforge.net/p/tmva/code/HEAD/tree/trunk/TMVA/macros/
[2]: https://github.com/nhanvtran/trackObservables/tree/master/analysis
[3]: http://uscms.org/uscms_at_work/physics/computing/setup/batch_systems.shtml#condor_1

