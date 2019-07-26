from os import listdir
from os.path import isfile, join
mypath = "/Users/naglelab/Downloads/summaryplots/"
onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f))]
text = "\\label{sec:yy}\n\\begin{figure}\n\\centering\n\\includegraphics[width=0.8\\textwidth]{summaryplots/yy}\n\\label{fig:summaryex}\n\\end{figure}"
output=[]
for f in onlyfiles:
	output.append("\n")
	output.append(text.replace("yy",f))
print("".join(output))
