REM
REM Register two datasets using Mutual Information
REM
set MRIDATA=c:/lorensen/InsightData/MultiFieldMRI
set MRIRegistration=c:/lorensen/Insight-VC++/Examples/MRIRegistration/RelWithDebInfo/MIRegistration.exe
echo 0 0 0 1 20 40 0 >guess0
%MRIRegistration%     --study2Prefix %MRIDATA%/HighField/e2     --study2Resolution 256 256 55     --study2Spacing .78125 .78125 1.5     --study1Prefix %MRIDATA%/LowField/e2     --study1Resolution 256 256 55     --study1Spacing .78125 .78125 1.5     --translateScale 64     --shrink 8 8 1     --learningRate .005     --standardDeviation 2     --numberOfSamples 100     --numberOfIterations 1000 <guess0 >guess1
%MRIRegistration%     --study2Prefix %MRIDATA%/HighField/e2     --study2Resolution 256 256 55     --study2Spacing .78125 .78125 1.5     --study1Prefix %MRIDATA%/LowField/e2     --study1Resolution 256 256 55     --study1Spacing .78125 .78125 1.5     --translateScale 64     --shrink 4 4 1     --learningRate .005     --standardDeviation 2     --numberOfSamples 100     --numberOfIterations 1000 <guess1 >guess2
%MRIRegistration%     --study2Prefix %MRIDATA%/HighField/e2     --study2Resolution 256 256 55     --study2Spacing .78125 .78125 1.5     --study1Prefix %MRIDATA%/LowField/e2     --study1Resolution 256 256 55     --study1Spacing .78125 .78125 1.5     --translateScale 64     --shrink 2 2 1     --learningRate .005     --standardDeviation 2     --numberOfSamples 100     --numberOfIterations 1000 <guess2 >guess3
%MRIRegistration%     --study2Prefix %MRIDATA%/HighField/e2     --study2Resolution 256 256 55     --study2Spacing .78125 .78125 1.5     --study1Prefix %MRIDATA%/LowField/e2     --study1Resolution 256 256 55     --study1Spacing .78125 .78125 1.5     --translateScale 64     --shrink 1 1 1     --learningRate .005     --standardDeviation 2     --numberOfSamples 100     --numberOfIterations 1000 <guess3 >guess4
