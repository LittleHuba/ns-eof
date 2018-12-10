
reynolds=[500, 1500, 5000, 10000]
steps=[(0.3, 0.4), (0.5, 0.6)]

if __name__ == '__main__':
	dim = 3
	top = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<configuration>\n"
	afterre = "    <simulation finalTime=\"10\" >\n        <type>turbulence</type>\n        <scenario>channel</scenario>\n    </simulation>\n"
	afterbfs = "    <timestep dt=\"1\" tau=\"0.5\" />\n    <solver gamma=\"0.5\" />\n"
	aftergeo = "      <mesh>uniform</mesh>\n    </geometry>\n    <environment gx=\"0\" gy=\"0\" gz=\"0\" />\n    <walls>\n        <left>\n            <vector x=\"1.0\" y=\"0\" z=\"0\" />\n        </left>\n        <right>\n            <vector x=\"0\" y=\"0\" z=\"0\" />\n        </right>\n        <top>\n            <vector x=\"0.0\" y=\"0.\" z=\"0.\" />\n        </top>\n        <bottom>\n            <vector x=\"0\" y=\"0\" z=\"0\" />\n        </bottom>\n        <front>\n            <vector x=\"0\" y=\"0\" z=\"0\" />\n        </front>\n        <back>\n            <vector x=\"0\" y=\"0\" z=\"0\" />\n        </back>\n    </walls>\n"
	aftervtk = "    <stdOut interval=\"0.01\" />\n"	
	end = "</configuration>\n"
	for re in reynolds:
		for step in steps:
			with open("conf_channel_turbulence_re_" + str(re) + "_x_" + str(step[0]) + "_y_" + str(step[1]) + ".xml", "w") as f:
				rey = "    <flow Re=\"{}\" />\n".format(re)
				bfs = "    <backwardFacingStep xRatio=\"{}\" yRatio=\"0.4\" />\n".format(step[0], step[1])
				geo = "    <geometry dim=\"{}\" lengthX=\"5.0\" lengthY=\"1.0\" lengthZ=\"1.0\" sizeX=\"{}\" sizeY=\"{}\" sizeZ=\"{}\">\n".format(dim, 50, 100, 50)
				vtk = "    <vtk interval=\"0.1\">channel_turbulence_no_vt_re_{}_steps_{},{}</vtk>\n".format(re, step[0], step[1])
				par = "    <parallel numProcessorsX=\"{}\" numProcessorsY=\"{}\" numProcessorsZ=\"{}\" />\n".format(2, 4, 2)

				f.write(top+rey+afterre+bfs+afterbfs+geo+aftergeo+vtk+aftervtk+par+end)


	top = "#!/bin/bash\n#SBATCH -D /home/hpc/t1221/lu26xut/no_vt/ns-eof #starting directory\n"
	mid = "#SBATCH --get-user-env #load user environment settings\n#SBATCH --clusters=mpp2\n"
	mid2 = "#SBATCH --mail-type=all\n#SBATCH --mail-user=ga53vuw@tum.de\n#SBATCH --export=NONE\n#SBATCH --time=04:00:00 #runtime\nsource /etc/profile.d/modules.sh #load modules defined in here\n\nfor i in 'seq 1 5'; do\n"
	end  = "done"

	for re in reynolds:
		for step in steps:
			with open("conf_channel_turbulence_re_" + str(re) + "_x_" + str(step[0]) + "_y_" + str(step[1]) + ".job", "w") as f:
				outName = "#SBATCH -o /home/hpc/t1221/lu26xut/no_vt/output/channel_re_{0}_x_{1}_y_{2}.out\n#SBATCH -J channel_tuburbulence_no_vt_{0}_x_{1}_y_{2} #name shown in squeue\n#SBATCH --ntasks={3}\n".format(re, step[0], step[1], 16)
				file = "    time mpiexec -np {} ./ns ".format(16) + "../config/conf_channel_turbulence_re_" + str(re) + "_x_" + str(step[0]) + "_y_" + str(step[1]) + ".xml\n"
				f.write(top+outName+mid+mid2+file+end)