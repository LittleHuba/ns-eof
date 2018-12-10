

if __name__ == '__main__':
	dim = 3
	top = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<configuration>\n    <flow Re=\"1000\" />\n    <simulation finalTime=\"5\" >\n        <type>dns</type>\n        <scenario>cavity</scenario>\n    </simulation>\n    <timestep dt=\"1\" tau=\"0.5\" />\n   <solver gamma=\"0.5\" />"
	mid = "      <mesh>uniform</mesh>\n    </geometry>\n    <environment gx=\"0\" gy=\"0\" gz=\"0\" />\n    <walls>\n        <left>\n            <vector x=\"0\" y=\"0\" z=\"0\" />\n        </left>\n        <right>\n            <vector x=\"0\" y=\"0\" z=\"0\" />\n        </right>\n        <top>\n            <vector x=\"1\" y=\"0\" z=\"0\" />\n        </top>\n        <bottom>\n            <vector x=\"0\" y=\"0\" z=\"0\" />\n        </bottom>\n        <front>\n            <vector x=\"0\" y=\"0\" z=\"0\" />\n        </front>\n        <back>\n            <vector x=\"0\" y=\"0\" z=\"0\" />\n        </back>\n    </walls>\n    <vtk interval=\"0.01\">cavity_result</vtk>\n    <stdOut interval=\"10\" />\n"
	bot = "</configuration>"

for i in range(1,5):
	with open("conf_cavity_weak_" + str(i) + "x" + str(i) + "x" + str(i) + ".xml", "w") as f:
		geo ="     <geometry dim=\"{}\" lengthX=\"1.0\" lengthY=\"1.0\" lengthZ=\"1.0\" sizeX=\"{}\" sizeY=\"{}\" sizeZ=\"{}\">\n".format(dim, i*10, i*10, i*10)
		par ="     <parallel numProcessorsX=\"{}\" numProcessorsY=\"{}\" numProcessorsZ=\"{}\" />\n".format(i,i,i)

		f.write(top+geo+mid+par+bot)

for i in range(1,5):
	with open("conf_cavity_strong_" + str(i) + "x" + str(i) + "x" + str(i) + ".xml", "w") as f:
		geo ="     <geometry dim=\"{}\" lengthX=\"1.0\" lengthY=\"1.0\" lengthZ=\"1.0\" sizeX=\"{}\" sizeY=\"{}\" sizeZ=\"{}\">\n".format(dim, 60, 60, 60)
		par ="     <parallel numProcessorsX=\"{}\" numProcessorsY=\"{}\" numProcessorsZ=\"{}\" />\n".format(i,i,i)

		f.write(top+geo+mid+par+bot)

dim = 2
for i in range(1,10):
	with open("conf_cavity_weak_" + str(i) + "x" + str(i) + ".xml", "w") as f:
		geo ="     <geometry dim=\"{}\" lengthX=\"1.0\" lengthY=\"1.0\" lengthZ=\"1.0\" sizeX=\"{}\" sizeY=\"{}\" sizeZ=\"{}\">\n".format(dim, i*10, i*10, 1)
		par ="     <parallel numProcessorsX=\"{}\" numProcessorsY=\"{}\" numProcessorsZ=\"{}\" />\n".format(i,i,1)

		f.write(top+geo+mid+par+bot)

for i in range(1,6):
	with open("conf_cavity_strong_" + str(i) + "x" + str(i) + ".xml", "w") as f:
		geo ="     <geometry dim=\"{}\" lengthX=\"1.0\" lengthY=\"1.0\" lengthZ=\"1.0\" sizeX=\"{}\" sizeY=\"{}\" sizeZ=\"{}\">\n".format(dim, 60, 60, 1)
		par ="     <parallel numProcessorsX=\"{}\" numProcessorsY=\"{}\" numProcessorsZ=\"{}\" />\n".format(i,i,1)

		f.write(top+geo+mid+par+bot)


	top = "#!/bin/bash\n#SBATCH -D /home/hpc/t1221/lu26xut/parallel/ns-eof #starting directory\n"
	mid = "#SBATCH --get-user-env #load user environment settings\n#SBATCH --clusters=mpp2\n"
	mid2 = "#SBATCH --mail-type=all\n#SBATCH --mail-user=ga53vuw@tum.de\n#SBATCH --export=NONE\n#SBATCH --time=04:00:00 #runtime\nsource /etc/profile.d/modules.sh #load modules defined in here\n\nfor i in 'seq 1 5'; do\n"
	end  = "done"

	for i in range(1,5):
		with open("conf_cavity_weak_" + str(i) + "x" + str(i) + "x" + str(i) + ".job", "w") as f:
			outName = "#SBATCH -o /home/hpc/t1221/lu26xut/parallel/output/Cavity_Weak_{0}x{0}x{0}.out\n#SBATCH -J weak_{0}x{0}x{0} #name shown in squeue\n#SBATCH --ntasks={1}\n".format(i, i*i*i)
			file = "    time mpiexec -np {} ./ns ".format(i*i*i) + "../config/conf_cavity_weak_" + str(i) + "x" + str(i) + "x" + str(i) + ".xml\n"
			f.write(top+outName+mid+mid2+file+end)

	for i in range(1,5):
		with open("conf_cavity_strong_" + str(i) + "x" + str(i) + "x" + str(i) + ".job", "w") as f:
			outName = "#SBATCH -o /home/hpc/t1221/lu26xut/parallel/output/Cavity_Strong_{0}x{0}x{0}.out\n#SBATCH -J strong_{0}x{0}x{0} #name shown in squeue\n#SBATCH --ntasks={1}\n".format(i, i*i*i)
			file = "    time mpiexec -np {} ./ns ".format(i*i*i) + "../config/conf_cavity_strong_" + str(i) + "x" + str(i) + "x" + str(i) + ".xml\n"
			f.write(top+outName+mid+mid2+file+end)

	for i in range(1,9):
		with open("conf_cavity_weak_" + str(i) + "x" + str(i) + ".job", "w") as f:
			outName = "#SBATCH -o /home/hpc/t1221/lu26xut/parallel/output/Cavity_Weak_{0}x{0}.out\n#SBATCH -J weak_{0}x{0} #name shown in squeue\n#SBATCH --ntasks={1}\n".format(i, i*i)
			file = "    time mpiexec -np {} ./ns ".format(i*i) + "../config/conf_cavity_weak_" + str(i) + "x" + str(i) + ".xml\n"
			f.write(top+outName+mid+mid2+file+end)

	for i in range(1,6):
		with open("conf_cavity_strong_" + str(i) + "x" + str(i) + ".job", "w") as f:
			outName = "#SBATCH -o /home/hpc/t1221/lu26xut/parallel/output/Cavity_Strong_{0}x{0}.out\n#SBATCH -J strong_{0}x{0} #name shown in squeue\n#SBATCH --ntasks={1}\n".format(i, i*i)
			file = "    time mpiexec -np {} ./ns ".format(i*i) + "../config/conf_cavity_strong_" + str(i) + "x" + str(i) + ".xml\n"
			f.write(top+outName+mid+mid2+file+end)