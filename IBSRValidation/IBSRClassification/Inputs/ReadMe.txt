This file shows the systax followed in the parameter files. 
The parameter files are needed for the Gaussian Classifier based applications involving 2 channels and are generated
apriori manually.

#NumberOfClasses (number of tissue type + 1 for background)
#0 #0 #1 #1 #2 #2 #3 #3 #4 #4 (each #corresponds to the mean of channel 1 & 2, for tissue type 1-4 & #0 for background)
#0 #0 #0 #0 #1 #1 #1 #1 #2 #2 #2 #2 #3 #3 #3 #3 #4 #4 #4 #4 (each group of 4 numbers corresponds to the covariances in two channel data, tissue type 1-4 & #0 for background)

#0 #1 #2 #3 #4 (each #corresponds to the number of samples of background, tissue type 1-4 in the model)

