# A Simple Graph-theory PNG File Generator

This project provides a simple tool to make a PNG file when you want to organize nodes and edges in a picture.

## What is it?

Give us some information, it can print a beautiful PNG file.

## License

GNU GPL v3.0

## How to install?

We use Ubuntu 16.04+ operating system.

First, you should install `npm` to get the plugin of this program, install `git` to clone our project:

```bash
apt update
apt install npm -y
apt install git -y
```

In order to manage our plugin, install `cnpm` :

```bash
npm install cnpm
```

Install plugin `convert-svg-to-png` :

```bash
cnpm install convert-svg-to-png
```

Now clone our project and make it:

```bash
cd /opt/
git clone https://github.com/wym6912/GraphtheoryGen.git
cd /opt/GraphtheoryGen/
make
```

## How to use?

### Mode Generator

We support 3 modes to view, so we provide this tool to get the mode in your input file:

```bash
cd /opt/GraphtheoryGen
./modegen
```

### Main tool

You should write an input file like this:

```text
3 4 5 <<<<<<< n nodes, m edges, mode
1     <<<<<<< Word printing on nodes (must be one or two character(s))
2     <<<<<<< Open this mode you want to print word,
3     <<<<<<< or omitted these lines.
1 2 1 <<<<<<< f t word, draw a line from f to t
2 3 4 <<<<<<< Open this mode if you want to print information on lines,
3 2 2 <<<<<<< or omitted word on every line.
1 2 3 <<<<<<< We also can display directed edges.
```

save it at `./input.txt`.

Use the program by this command:

```bash
graphgen ./input.txt
```

you will get an information like that:

```text
Generated config file config-graph.
Please confirm this file and restart this program.
```

then open `config-graph` :

```ini
[Files]
# Set temp file "xxx.svg" and export file will be named by "xxx.png"
TmpFile=export.svg
# Set the height and width of png file
Height=600
Width=1000
# If multipled edges in your picture, it can offset your multiple edges to prevent match together these edges.
MultipleEdgeOffset=30.000000
# Set the radius of node circle
CicrleRadius=19.000000
# Set text offset in nodes
CicrleText1dx=-5.500000
CicrleText1dy=5.500000
CicrleText1=11.000000
CicrleText2dx=-9.000000
CicrleText2dy=6.000000
CicrleText2=18.000000
# Set angle of connecting edges
CicrleAngle0=-45.000000
CicrleAngle1=-90.000000
CicrleAngle2=-135.000000
CicrleAngle3=180.000000
CicrleAngle4=135.000000
CicrleAngle5=90.000000
CicrleAngle6=45.000000
CicrleAngle7=0.000000
# remove svg file after converting png file
RemoveTempFile=true
[Node-place]
# Set the places of every nodes
Pointx1=500
Pointy1=100
Pointx2=300
Pointy2=200
Pointx3=700
Pointy3=200
Pointx4=200
Pointy4=300
Pointx5=400
Pointy5=300
Pointx6=600
Pointy6=300
Pointx7=800
Pointy7=300
Pointx8=100
Pointy8=400
Pointx9=300
Pointy9=400
Pointx10=500
Pointy10=400
```

and rerun it:

```text
graphgen ./input.txt
```

you will get a PNG file.

## Third-party 

We use [`INIFile config`](https://github.com/Winnerhust/inifile2), [`tinyxml2`](https://github.com/leethomason/tinyxml2) and [`convert-svg-to-png`](https://github.com/TheZyppi/convert-svg-to-png).
