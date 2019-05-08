#include "honoka.hpp"
#include "tinyxml2.cpp" 
#include "inifile.cpp"

using namespace tinyxml2;
using namespace inifile;

//Data Segment Begin

//Constant Varibles
const int MAXPOINTS = 10;
const int MAXEDGES = 100;
const int SUPPORTED_MODE = 3;
const int MODE_NODE = 1, MODE_EDGE = 2, MODE_DIRECTED = 3;
//Placing points
const int PLACE_X[MAXPOINTS + 1] = {0, 500, 300, 700, 200, 400, 600, 800, 100, 300, 500};
const int PLACE_Y[MAXPOINTS + 1] = {0, 100, 200, 200, 300, 300, 300, 300, 400, 400, 400};

//File Information
string tempfile;
bool RemoveSVG;

//Graph Information
int n, m, mode;
int matrix[MAXPOINTS + 1][MAXPOINTS + 1], drawed_times[MAXPOINTS + 1][MAXPOINTS + 1];
string NodeInformation[MAXPOINTS + 1];

struct Edge {int from, to; string Information;} EdgeInformation[MAXEDGES + 1];

map <string, int> InformationToID, InformationToEdge;

XMLDocument SVGstream;

//Place Information
IniFile Config;
int Height, Width;
int PlaceX[MAXPOINTS + 1], PlaceY[MAXPOINTS + 1];
XMLElement *Elementroot, *ElementPoint[MAXPOINTS + 1], *ElementEdge[MAXEDGES + 1],
           *TextPoint[MAXPOINTS + 1], *Love, *Arrow, *Shoot, 
		   *TextEdge[MAXEDGES + 1], *TextPathEdge[MAXEDGES + 1];
double CircleRadius, CircleAngle[8], MultipleEdgeOffset;

struct CircleFillInfo {double dx, dy, length;} CircleFill[2];

//Data Segment End

inline int ModeDispatch(int Mode)
{
	return 1 << (Mode - 1);
}

inline int GetTwoPointsRelation(int x, int y)
{
	/*------------------------------*\
	|Calcuate two points place:		 |
	|(from point x to point y)		 |
	| 0: left and up				 |
	| 1: up							 |
	| 2: right and up				 |
	| 3: right						 |
	| 4: right and down				 |
	| 5: down						 |
	| 6: left and down				 |
	| 7: left						 |
	\*------------------------------*/
	if(PlaceX[x] < PlaceX[y])
	{
		if(PlaceY[x] < PlaceY[y]) {return 4;}
		else if(PlaceY[x] == PlaceY[y]) {return 3;}
		else if(PlaceY[x] > PlaceY[y]) {return 2;}
		else __assert__(false, "Please contact developers. (GetTwoPointsRelation1)");
	}
	else if(PlaceX[x] == PlaceX[y])
	{
		if(PlaceY[x] < PlaceY[y]) {return 5;}
		else if(PlaceY[x] == PlaceY[y]) {return -1;}
		else if(PlaceY[x] > PlaceY[y]) {return 1;}
		else __assert__(false, "Please contact developers. (GetTwoPointsRelation2)");
	}
	else if(PlaceX[x] > PlaceX[y])
	{
		if(PlaceY[x] < PlaceY[y]) {return 6;}
		else if(PlaceY[x] == PlaceY[y]) {return 7;}
		else if(PlaceY[x] > PlaceY[y]) {return 0;}
		else __assert__(false, "Please contact developers. (GetTwoPointsRelation3)");
	}
	else __assert__(false, "Please contact developers. (GetTwoPointsRelation0)");
	return -1;
}

inline bool NeedtoRotate(int x, int y)
{
	if(PlaceX[x] < PlaceX[y]) return false;
	return true;
}

inline void ParseArg(int &argc, char *argv[])
{
	if(argc < 2 || argc >= 4)
	{
		cerr << "Usage: " << argv[0] << " <program rule file name>";
		cerr << "\nIf you don\'t know the rule, please read the manual file.\n";
		exit(1);
	}
}

inline void GenerateINI()
{
	IniFile INI;
	INI.SetStringValue("Files", "TmpFile", "export.svg");
	INI.SetIntValue("Files", "Height", 600);
	INI.SetIntValue("Files", "Width", 1000);
	INI.SetDoubleValue("Files", "MultipleEdgeOffset", 30.0);
	INI.SetDoubleValue("Files", "CicrleRadius", 19.0);
	INI.SetDoubleValue("Files", "CicrleText1dx", -5.5);
	INI.SetDoubleValue("Files", "CicrleText1dy", 5.5);
	INI.SetDoubleValue("Files", "CicrleText1", 11.0);
	INI.SetDoubleValue("Files", "CicrleText2dx", -9.0);
	INI.SetDoubleValue("Files", "CicrleText2dy", 6.0);
	INI.SetDoubleValue("Files", "CicrleText2", 18.0);
	INI.SetDoubleValue("Files", "CicrleAngle0", -45.0);
	INI.SetDoubleValue("Files", "CicrleAngle1", -90.0);
	INI.SetDoubleValue("Files", "CicrleAngle2", -135.0);
	INI.SetDoubleValue("Files", "CicrleAngle3", 180.0);
	INI.SetDoubleValue("Files", "CicrleAngle4", 135.0);
	INI.SetDoubleValue("Files", "CicrleAngle5", 90.0);
	INI.SetDoubleValue("Files", "CicrleAngle6", 45.0);
	INI.SetDoubleValue("Files", "CicrleAngle7", 0.0);
	INI.SetStringValue("Files", "RemoveTempFile", "true");
	string pointstring, X = "Pointx", Y = "Pointy";
	for(register int i = 1; i <= MAXPOINTS; ++ i)
	{
		pointstring = EatBread(i);
		INI.SetIntValue("Node-place", X + pointstring, PLACE_X[i]);
		INI.SetIntValue("Node-place", Y + pointstring, PLACE_Y[i]);
	}
	INI.SaveAs("config-graph");
}

inline bool CheckandWriteINIToThisProgram()
{
	if(Config.GetStringValue("Files", "TmpFile", &tempfile) == ERR_NOT_FOUND_KEY)
		return false;
	if(Config.GetIntValue("Files", "Height", &Height) == ERR_NOT_FOUND_KEY)
		return false;
	if(Config.GetIntValue("Files", "Width", &Width) == ERR_NOT_FOUND_KEY)
		return false;
	if(Config.GetDoubleValue("Files", "CicrleRadius", &CircleRadius) == ERR_NOT_FOUND_KEY)
		return false;
	if(Config.GetDoubleValue("Files", "CicrleText1dx", &CircleFill[0].dx) == ERR_NOT_FOUND_KEY)
		return false;
	if(Config.GetDoubleValue("Files", "CicrleText1dy", &CircleFill[0].dy) == ERR_NOT_FOUND_KEY)
		return false;
	if(Config.GetDoubleValue("Files", "CicrleText1", &CircleFill[0].length) == ERR_NOT_FOUND_KEY)
		return false;
	if(Config.GetDoubleValue("Files", "CicrleText2dx", &CircleFill[1].dx) == ERR_NOT_FOUND_KEY)
		return false;
	if(Config.GetDoubleValue("Files", "CicrleText2dy", &CircleFill[1].dy) == ERR_NOT_FOUND_KEY)
		return false;
	if(Config.GetDoubleValue("Files", "CicrleText2", &CircleFill[1].length) == ERR_NOT_FOUND_KEY)
		return false;
	if(Config.GetDoubleValue("Files", "MultipleEdgeOffset", &MultipleEdgeOffset) == ERR_NOT_FOUND_KEY)
		return false;
		
	string Getstring, IntNumber;
	if(Config.GetStringValue("Files", "RemoveTempFile", &Getstring) == ERR_NOT_FOUND_KEY)
		return false;
	if(Getstring == "true") RemoveSVG = true;
	else if(Getstring == "false") RemoveSVG = false;
	else return false;
	
	for(register int i = 0; i < 8; ++ i)
	{
		IntNumber = EatBread(i);
		IntNumber = "CicrleAngle" + IntNumber;
		if(Config.GetDoubleValue("Files", IntNumber, &CircleAngle[i]) == ERR_NOT_FOUND_KEY)
			return false;
	}
	
	string X = "Pointx", Y = "Pointy";
	for(register int i = 1; i <= MAXPOINTS; ++ i)
	{
		IntNumber = EatBread(i);
		if(Config.GetIntValue("Node-place", X + IntNumber, PlaceX + i) 
		                   == ERR_NOT_FOUND_KEY)
			return false;
		if(Config.GetIntValue("Node-place", Y + IntNumber, PlaceY + i)
		                   == ERR_NOT_FOUND_KEY)
			return false;
	}
	return true;
}

inline void ReadINI()
{
	if(Config.Load("config-graph") == ERR_OPEN_FILE_FAILED)
	{
		GenerateINI();
		cerr << "Generated config file config-graph.\n";
		cerr << "Please confirm this file and restart this program." << endl;
		exit(3);
	}
	if(! CheckandWriteINIToThisProgram())
	{
		cerr << "Please delete config-graph and restart this program." << endl;
		exit(4);
	}
}

inline void DispatchInput(string arg)
{
	ifstream fi(arg);
	if(! fi.is_open())
	{
		cerr << "Can\'t open Input File: " << arg << "\n";
		exit(2);
	}
	__assert__(fi >> n >> m >> mode, "Error in reading Input File. (Line 1)");
	__assert__(1 <= n && n <= MAXPOINTS, "Please confirm the number of points, or enlarge the MaxPoints and recomplie this program.");
	__assert__(0 <= m && m <= MAXEDGES, "Please confirm the number of edges, or enlarge the MaxEdges and recomplie this program.");
	__assert__(0 <= mode && mode < ModeDispatch(SUPPORTED_MODE + 1), "Please confirm the mode, mode is in [0, 8)");
	if(mode & ModeDispatch(MODE_NODE))
	{
		//Nodes have extra information
		for(register int i = 1; i <= n; ++ i)
		{
			__assert__(fi >> NodeInformation[i], "Error in reading Input File. (Line 2)");
			InformationToID[NodeInformation[i]] = i;
		}
	}
	for(register int i = 1, 
	                 hasExtraInformation = mode & ModeDispatch(MODE_EDGE); 
					 i <= m; ++ i)
	{
		__assert__(fi >> EdgeInformation[i].from >> EdgeInformation[i].to,
		           "Error in reading Input File. (Line 3)");
		if(hasExtraInformation) 
		{
			//Edges have extra information
			__assert__(fi >> EdgeInformation[i].Information, 
			           "Error in reading Input File. (Line 3)");
			InformationToEdge[EdgeInformation[i].Information] = i;
		}
		//Consider Multiple Edges
		++ matrix[EdgeInformation[i].from][EdgeInformation[i].to];
		++ matrix[EdgeInformation[i].to][EdgeInformation[i].from];
	}
}

inline void GenerateSVG()
{
	//Deal with SVGstream
	Elementroot = SVGstream.NewElement("svg");
	Elementroot -> SetAttribute("xmlns", "http://www.w3.org/2000/svg");
	Elementroot -> SetAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
	SVGstream.InsertEndChild(Elementroot);
	
	//Generate Circle
	for(register int i = 1, FillMode = mode & ModeDispatch(MODE_NODE); i <= n; ++ i)
	{
		ElementPoint[i] = SVGstream.NewElement("circle");
		ElementPoint[i] -> SetAttribute("cx", PlaceX[i]);
		ElementPoint[i] -> SetAttribute("cy", PlaceY[i]);
		ElementPoint[i] -> SetAttribute("r", CircleRadius);
		ElementPoint[i] -> SetAttribute("fill", "white");
		ElementPoint[i] -> SetAttribute("stroke", "black");
		Elementroot -> InsertEndChild(ElementPoint[i]);
		if(FillMode) //Filled with Extra Information
		{
			TextPoint[i] = SVGstream.NewElement("text");
			TextPoint[i] -> SetAttribute("x", PlaceX[i]);
			TextPoint[i] -> SetAttribute("y", PlaceY[i]);
			if(NodeInformation[i].size() == 1)
			{
				TextPoint[i] -> SetAttribute("dx", CircleFill[0].dx);
				TextPoint[i] -> SetAttribute("dy", CircleFill[0].dy);
				TextPoint[i] -> SetAttribute("textLength", CircleFill[0].length);
			}
			else if(NodeInformation[i].size() == 2)
			{
				TextPoint[i] -> SetAttribute("dx", CircleFill[1].dx);
				TextPoint[i] -> SetAttribute("dy", CircleFill[1].dy);
				TextPoint[i] -> SetAttribute("textLength", CircleFill[1].length);
			}
			else
			{
				__assert__(false, "Error in writing lines, \
				                   Please confirm your Node displaying. \
								   It must be one or two character(s).");
			}
			TextPoint[i] -> SetText(NodeInformation[i].c_str());
			Elementroot -> InsertEndChild(TextPoint[i]);
		}
	}
	
	//Generate Arrow if viewed directed edges
	if(! (mode & ModeDispatch(MODE_DIRECTED)))
	{
		Love = SVGstream.NewElement("defs");
		
		Arrow = SVGstream.NewElement("marker");
		Arrow -> SetAttribute("id", "LoveArrowShoot");
		Arrow -> SetAttribute("viewBox", "0 0 12 12");
		Arrow -> SetAttribute("refX", 6);
		Arrow -> SetAttribute("refY", 6);
		Arrow -> SetAttribute("markerUnits", "strokeWidth");
		Arrow -> SetAttribute("markerWidth", 12);
		Arrow -> SetAttribute("markerHeight", 12);
		Arrow -> SetAttribute("orient", "auto");
		
		Shoot = SVGstream.NewElement("path");
		Shoot -> SetAttribute("d", "M 2 2 L 10 6 L 2 10 L 8 6 L 2 2");
		Shoot -> SetAttribute("fill", "black");
		
		Love -> InsertEndChild(Arrow);
		Arrow -> InsertEndChild(Shoot);
		
		Elementroot -> InsertEndChild(Love);
	}
	
	string OutputWay;
	//Generate Lines
	for(register int i = 1, 
	                 UnDirected = mode & ModeDispatch(MODE_DIRECTED),
	                 hasExtraInformation = mode & ModeDispatch(MODE_EDGE),
	                 f, t, TwoPointsRelation; i <= m; ++ i)
	{//for
		f = EdgeInformation[i].from, t = EdgeInformation[i].to;
		//Calcuate two points drawed times
		++ drawed_times[f][t], ++ drawed_times[t][f];
		
		ElementEdge[i] = SVGstream.NewElement("path");
		
		OutputWay = "";
		TwoPointsRelation = GetTwoPointsRelation(f, t);
		//Notice: The coordinate of points is abnormal.
		double MoveStartX = double(PlaceX[f]) - CircleRadius * __cos__(CircleAngle[TwoPointsRelation]), 
			   MoveStartY = double(PlaceY[f]) + CircleRadius * __sin__(CircleAngle[TwoPointsRelation]),
			   MoveEndX = double(PlaceX[t]) - CircleRadius * __cos__(CircleAngle[(TwoPointsRelation + 4) % 8]), 
			   MoveEndY = double(PlaceY[t]) + CircleRadius * __sin__(CircleAngle[(TwoPointsRelation + 4) % 8]);
		OutputWay = "M " + EatBread(MoveStartX) + " " + EatBread(MoveStartY) + " ";
		
		if(matrix[f][t] == 1 || (drawed_times[f][t] == 1 && (matrix[f][t] & 1)))
		{//if(matrix[][] == 1)
			//Draw only one line or draw the first of (3, 5, 7...) lines
			OutputWay += "L " + EatBread(MoveEndX) + " " + EatBread(MoveEndY);
			
			ElementEdge[i] -> SetAttribute("stroke", "black");
			ElementEdge[i] -> SetAttribute("d", OutputWay.c_str());
			OutputWay = "Printemps" + EatBread(i);
			ElementEdge[i] -> SetAttribute("id", OutputWay.c_str());
			
			if(! UnDirected) //Directed Lines, drawing arrows. (Love arrow shoot!)
			{
				ElementEdge[i] -> SetAttribute("marker-end", "url(#LoveArrowShoot)"); 
			}
			Elementroot -> InsertEndChild(ElementEdge[i]);
			
			if(hasExtraInformation) //Consider More Information printing on lines.
			{//hasExtraInformation
				TextEdge[i] = SVGstream.NewElement("text");
				TextEdge[i] -> SetAttribute("text-anchor", "middle");
				if(NeedtoRotate(f, t)) 
				{
					TextEdge[i] -> SetAttribute("rotate", "180");
					reverse(EdgeInformation[i].Information.begin(), EdgeInformation[i].Information.end());
				}
				TextPathEdge[i] = SVGstream.NewElement("textPath");
				TextPathEdge[i] -> SetAttribute("xlink:href", ("#" + OutputWay).c_str());
				//TextPathEdge[i] -> SetAttribute("textLength", "300");
				//Make it middle
				TextPathEdge[i] -> SetAttribute("startOffset", "50%");
				TextPathEdge[i] -> SetText(EdgeInformation[i].Information.c_str());
				
				Elementroot -> InsertEndChild(TextEdge[i]);
				TextEdge[i] -> InsertEndChild(TextPathEdge[i]);
			}//hasExtraInformation
		}//if(matrix[][] == 1)
		else 
		{//else if(matrix[][] != 1)
			if((matrix[f][t] - drawed_times[f][t]) & 1)
			{//if(matrix[f][t] - drawed_times[f][t]) & 1
				//Draw upper curve
				OutputWay += "S " + EatBread((MoveStartX + MoveEndX) / 2.0 + MultipleEdgeOffset * (matrix[f][t] - drawed_times[f][t]))
                            + " " + EatBread((MoveStartY + MoveEndY) / 2.0 + MultipleEdgeOffset * (matrix[f][t] - drawed_times[f][t]));
				OutputWay += " " + EatBread(MoveEndX) + " " + EatBread(MoveEndY);
				ElementEdge[i] -> SetAttribute("stroke", "black");
				ElementEdge[i] -> SetAttribute("fill", "transparent");
				ElementEdge[i] -> SetAttribute("d", OutputWay.c_str());
				OutputWay = "BiBi" + EatBread(i);
				ElementEdge[i] -> SetAttribute("id", OutputWay.c_str());
				if(! UnDirected) //Directed Lines, drawing arrows. (Love arrow shoot!)
				{
					ElementEdge[i] -> SetAttribute("marker-end", "url(#LoveArrowShoot)");
				}
				Elementroot -> InsertEndChild(ElementEdge[i]);
				
				if(hasExtraInformation) //Consider More Information printing on lines.
				{
					TextEdge[i] = SVGstream.NewElement("text");
					TextEdge[i] -> SetAttribute("text-anchor", "middle");
					if(NeedtoRotate(f, t)) 
					{
						TextEdge[i] -> SetAttribute("rotate", "180");
						reverse(EdgeInformation[i].Information.begin(), EdgeInformation[i].Information.end());
					}
					TextPathEdge[i] = SVGstream.NewElement("textPath");
					TextPathEdge[i] -> SetAttribute("xlink:href", ("#" + OutputWay).c_str());
					//TextPathEdge[i] -> SetAttribute("textLength", "300");
					//Make it middle
					TextPathEdge[i] -> SetAttribute("startOffset", "50%");
					TextPathEdge[i] -> SetText(EdgeInformation[i].Information.c_str());
					
					Elementroot -> InsertEndChild(TextEdge[i]);
					TextEdge[i] -> InsertEndChild(TextPathEdge[i]);
				}
			}//if (matrix[f][t] - drawed_times[f][t]) & 1
			else 
			{//else (matrix[f][t] - drawed_times[f][t]) != 1
				//Draw lower curve 
				OutputWay += "S " + EatBread((MoveStartX + MoveEndX) / 2.0 - MultipleEdgeOffset * (matrix[f][t] - drawed_times[f][t] + 1.0))
                            + " " + EatBread((MoveStartY + MoveEndY) / 2.0 - MultipleEdgeOffset * (matrix[f][t] - drawed_times[f][t] + 1.0));
				OutputWay += " " + EatBread(MoveEndX) + " " + EatBread(MoveEndY);
				ElementEdge[i] -> SetAttribute("stroke", "black");
				ElementEdge[i] -> SetAttribute("fill", "transparent");
				ElementEdge[i] -> SetAttribute("d", OutputWay.c_str());
				OutputWay = "Lilywhite" + EatBread(i);
				ElementEdge[i] -> SetAttribute("id", OutputWay.c_str());
				if(! UnDirected) //Directed Lines, drawing arrows. (Love arrow shoot!)
				{
					ElementEdge[i] -> SetAttribute("marker-end", "url(#LoveArrowShoot)"); 
				}
				Elementroot -> InsertEndChild(ElementEdge[i]);
				
				if(hasExtraInformation) //Consider More Information printing on lines.
				{
					TextEdge[i] = SVGstream.NewElement("text");
					TextEdge[i] -> SetAttribute("text-anchor", "middle");
					if(NeedtoRotate(f, t))
					{ 
						TextEdge[i] -> SetAttribute("rotate", "180");
						reverse(EdgeInformation[i].Information.begin(), EdgeInformation[i].Information.end());
					}
					TextPathEdge[i] = SVGstream.NewElement("textPath");
					TextPathEdge[i] -> SetAttribute("xlink:href", ("#" + OutputWay).c_str());
					//TextPathEdge[i] -> SetAttribute("textLength", "300");
					//Make it middle
					TextPathEdge[i] -> SetAttribute("startOffset", "50%");
					TextPathEdge[i] -> SetText(EdgeInformation[i].Information.c_str());
					
					Elementroot -> InsertEndChild(TextEdge[i]);
					TextEdge[i] -> InsertEndChild(TextPathEdge[i]);
				}
			}//! (matrix[f][t] - drawed_times[f][t]) & 1
		}//else if(matrix[][] != 1)
		
	}//for
	
}

inline void WriteToSVG(string &arg)
{
	ofstream fo(arg);
	if(! fo.is_open())
	{
		cerr << "Can\'t create temporaty File: " << arg << "\n";
		exit(2);
	}
	SVGstream.SaveFile(arg.c_str());
	cout << "SVG File writed to temporaty file Successed.\n";
}

inline void ConvertToPNG()
{
	string Command = "convert-svg-to-png ";
	Command += tempfile + " ";
	Command += "--height " + EatBread(Height) + " ";
	Command += "--width " + EatBread(Width);
	system(Command.c_str());
}

inline void RemoveSVGFile()
{
	system(("rm " + tempfile).c_str());
}

int main(int argc, char *argv[])
{
	ParseArg(argc, argv);
	ReadINI();
	DispatchInput(argv[1]);
	GenerateSVG();
	WriteToSVG(tempfile);
	ConvertToPNG();
	if(RemoveSVG) RemoveSVGFile();
	return 0;
}
