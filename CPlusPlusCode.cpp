// optimisation algorithm to find the shortest distance to travel to the top 100 most populous places in the UK
//streams imported
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <cmath>
#include <vector>
#include <string>
#define PI atan(1)*4

/*Improvements made: 
-weighted and non weighted distances, by population, found
-the total distance found for both, and the extra distance travelled is found (when considering population)
-the nearest city to the weighted and non weighted co ordinates is identified
*/

//using namespace shortcut
using namespace std;

//define functions
//random number generator
double random_number(double upper, double lower, int n) {
	double r;
	r = lower + (rand() % (n + 1) * (1. / n) * (upper - lower));
	return r;
}
//distance calculator
double distance_calculator(double long1, double lat1, double x, double y, double w) {
	double dLat = (y * (PI / 180)) - (lat1 * (PI / 180));
	double dLong = (x * (PI / 180)) - (long1 * (PI / 180));
	double length_1 = pow(sin(dLat / 2), 2) + (cos(x * (PI / 180))*cos(long1 * (PI / 180))*pow(sin(dLong / 2), 2));
	double length_2 = 2 * atan2(sqrt(length_1), sqrt(1 - length_1));
	double final_step = 3958.75*length_2*(w);
	return final_step;
}
//main program starts here
int main() {
	//***********************Section 1: File is opened, read from and closed here************************************************************
	// declare variables to be used in splitting the data csv file, separated by columns and commas, into a vector
	string line;
	string xtemp, ytemp;
	int comma;
	int commaNew;
	int commaOld;
	//creates a vector data structure to store the data
	vector<vector<double> > data;

	// opens up the file and reads in the data
	ifstream dataFile("GBPlaces.csv");

	// if file has been opened, then a message is printed telling user it opened successfully
	if (dataFile.is_open()) {
		cout << "File has been opened successfully! Please wait...\n";
		// while the final line in the file has not been reached while reading it
		while (!dataFile.eof()) {
			// data in text file is read in line by line
			getline(dataFile, line);
			if (line.find("%", 0) == -1){
				//ensures there's data in the line 
				if (line.length() > 0){
					//using a temporary vector to hold the data 
					vector <double> temp;
					//sets a counter so initial value for comma is 0
					comma = 0;
					commaNew = 0;
					//finds last comma in the line of the file concerned
					commaOld = line.find_last_of(",");
					//have the vector hold all commas and values
					while (commaNew != commaOld) {
						// the items between the commas are extracted
						commaNew = line.find(",", comma);
						xtemp = line.substr(comma, commaNew - comma);
						comma = commaNew + 1;
						// these items are pushed onto the temp vector
						temp.push_back(atof(xtemp.c_str()));
					}
					// obtains the final item after the comma
					xtemp = line.substr(commaOld + 1, line.length() - commaOld - 1);
					// this final value for each line is added back onto the temp vector
					temp.push_back(atof(xtemp.c_str()));

					//pushes all values into the data structure vector
					data.push_back(temp);
				}
			}
		}
		// closes file
		dataFile.close();
	}
	//separate vector created holding the string values for the list of places
	vector<vector<string> > places;
	// opens up the file and reads in the data
	ifstream placesFile("GBPlaces.csv");

	// if file is opeend, a message is printed
	if (placesFile.is_open()) {
		while (!placesFile.eof()) {
			getline(placesFile, line);
			if (line.find("%", 0) == -1){
				vector <string> temp2;
				//ensures there's data in the line 
				if (line.length() > 0){
					//using a temporary vector to hold the data 
					commaOld = line.find(",", 0);
					xtemp = line.substr(0, commaOld);
					temp2.push_back(xtemp.c_str());
				}
				xtemp = line.substr(0, ',');
				temp2.push_back(xtemp.c_str());

				//pushes all values into the data structure vector
				places.push_back(temp2);
			}
		}
		// closes file
		placesFile.close();
	}
	else {
		// error message printed if file unable to open
		cout << "Unable to open file\n";
		exit(1);
	}
	//maximum and minimum values for latitude and longitude in the data structure are searched for to be used later when calculating a random number in the range
	double max_latitude = 0;
	for (int i = 0; i < data.size(); i++){
		if (data[i][3] > max_latitude){
			max_latitude = data[i][3];
		}
	}
	double max_longitude = 0;
	for (int i = 0; i < data.size(); i++){
		if (data[i][4] > max_longitude){
			max_longitude = data[i][4];
		}
	}
	double min_longitude = 0;
	for (int i = 0; i < data.size(); i++){
		if (data[i][4] < min_longitude){
			min_longitude = data[i][4];
		}
	}
	double min_latitude = 0;
	for (int i = 0; i < data.size(); i++){
		if (data[i][3] < min_latitude){
			min_latitude = data[i][3];
		}
	}
	//*****************************Section 2: Random point found, co ordinates and total distance found after optimisation (neglecting population size)*************************************************************************
	// declare variables to be used in finding the optimised location, in both cases (non weighted and weighted)
	double x, y;
	double glx, gly;
	double glx_w, gly_w;
	int dx, dy;
	int dx_w, dy_w;
	double step = 0.01;
	double value, newValue, oldValue;
	double value_w, newValue_w, oldValue_w;
	double globalMin = 1000000000;
	double globalMin_w = 1000000000;
	double distance;
	double distance_w;
	srand(time(NULL));

	//starts evaluations/iterations at 0
	int fevals = 0;
	// random starting point for x (longitude) and y (latitude) picked within range of dataset
	for (int k = 0; k < 50; k++) {
		x = random_number(max_longitude, min_longitude, 100);
		y = random_number(max_latitude, min_latitude, 100);
		distance = 0;
		// Haversine formula used to find distances between places for this random value of x and y
		for (int i = 0; i < data.size(); i++){
			double longitude = data[i][4];
			double latitude = data[i][3];
			double x_rand = x;
			double y_rand = y;
			double weight = 1;
			value = distance_calculator(longitude, latitude, x_rand, y_rand, weight);
			distance += value;
		}
		do {
			// this bit looks around the current point to see if the sum of all distances is lower
			oldValue = distance;

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (i == 0 && j == 0) {
					}
					else {
						newValue = 0;
						for (int l = 0; l < data.size(); l++){
							double longitude = data[l][4];
							double latitude = data[l][3];
							double x_rand = x;
							double y_rand = y;
							double weight = 1;
							newValue += distance_calculator(longitude, latitude, x + step * i, y + step * j, weight);
						}
						if (newValue <= distance) {
							dx = i;
							dy = j;
							distance = newValue;
						}
					}
				}
			}
			// updates values for x and y to this new position
			x += step * dx;
			y += step * dy;
			//adds one to the evaulations/iterations counter
			fevals++;
		} while (distance < oldValue);

		//global minimum found which is the smallest distance available
		if (distance < globalMin) {
			globalMin = distance;
			glx = x;
			gly = y;
		}
		//************************Section 3: Random point found, co ordinates and total distance found after optimisation acknowledging population size*************************************************************************
		distance_w = 0;
		// Haversine formula used to find distances between places for this random value of x and y, but now the weight of each value depends on population
		for (int i = 0; i < data.size(); i++){
			double longitude = data[i][4];
			double latitude = data[i][3];
			double x_rand = x;
			double y_rand = y;
			double weight = 1 / data[i][2];
			value_w = distance_calculator(longitude, latitude, x_rand, y_rand, weight);
			distance_w += value_w;
		}
		do {
			// looks around current value to see if the total distance to all the other places is decreased
			oldValue_w = distance_w;

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (i == 0 && j == 0) {
					}
					else {
						newValue_w = 0;
						for (int l = 0; l < data.size(); l++){
							double longitude = data[l][4];
							double latitude = data[l][3];
							double x_rand = x;
							double y_rand = y;
							double weight = 1 / data[l][2];
							newValue_w += distance_calculator(longitude, latitude, x + step * i, y + step * j, weight);
						}
						if (newValue_w <= distance_w) {
							dx_w = i;
							dy_w = j;
							distance_w = newValue_w;
						}
					}
				}
			}
			// updates values of positions for these new co ordinates
			x += step * dx_w;
			y += step * dy_w;

		} while (distance_w < oldValue_w);
		if (distance_w < globalMin_w) {
			globalMin_w = distance_w;
			glx_w = x;
			gly_w = y;
		}
		globalMin_w = 0;
		//calculates total distance using these new weighted co ordinates
		for (int i = 0; i < data.size(); i++){
			double longitude = data[i][4];
			double latitude = data[i][3];
			double x = glx_w;
			double y = gly_w;
			double weight = 1;
			value_w = distance_calculator(longitude, latitude, x, y, weight);
			globalMin_w += value_w;
		}
		//prints  the co ordinates for weighted and non weighted values for each iteration
		cout << "Evaluations: " << fevals << ". Original: " << gly << "," << glx << ". Weighted: " << gly_w << "," << glx_w << endl;
	}
	//***************Section 4: Finds the nearest location to these set of global co ordinate values, for weighted and non weighted***************************************************************************************
	//distance calculator used to find the distance for the non weighted co ordinates to all other places
	double closestDistance;
	//vector generated to hold the values for the distances from the co ordinates to other locations
	vector<double>closestPositionDistance;
	for (int i = 0; i < data.size(); i++){
		double longitude = data[i][4];
		double latitude = data[i][3];
		double x = glx;
		double y = gly;
		double weight = 1;
		closestDistance = distance_calculator(longitude, latitude, x, y, weight);
		closestPositionDistance.push_back(closestDistance);
	}
	//index of the element in the list with the smallest distance identified, to be used in finding the nearest city/town later
	int closestPosition = 0;
	for (int i = 0; i < data.size(); ++i)
	{
		if (closestPositionDistance[i] < closestPositionDistance[closestPosition])
			closestPosition = i;
	}
	//distance calculator used to find distance for the weighted co ordinates to all other places
	double closestDistance_w;
	//vector generated to hold the values for weighted distances from the co ordianates to the other locations
	vector<double>closestPositionDistance_w;
	for (int i = 0; i < data.size(); i++){
		double longitude = data[i][4];
		double latitude = data[i][3];
		double x = glx_w;
		double y = gly_w;
		double weight = 1;
		closestDistance_w = distance_calculator(longitude, latitude, x, y, weight);
		closestPositionDistance_w.push_back(closestDistance_w);
	}
	//index of the element in the list with the smallest weighted distance identified, to be used in finding the nearest city/town later
	int closestPosition_w = 0;
	for (int i = 0; i < data.size(); ++i)
	{
		if (closestPositionDistance_w[i] < closestPositionDistance_w[closestPosition_w]) // Found a smaller min
			closestPosition_w = i;
	}
	//************************************Section 5: Distances and differences calculated******************************************************************************************
	//calculates difference between the two global minima for weighted and non weighted distances
	double distance_difference = globalMin_w - globalMin;
	//calculates total distance for the non weighted and weighted global minima (taking into account the fact that each distance needs to be travelled twice- i.e. to and from the global co ordinates to each city)
	double total_distance = 2 * globalMin;
	double total_distance_w = 2 * globalMin_w;
	//************************************Section 6: Results for both are outputted******************************************************************************************
	//prints a formatted output giving the co ordinates for weighted and non weighted positions, as well as the total distance travelled
	printf("\nCo ordinates (non weighted for population): %6.2f,%6.2f degrees. \nTotal non weighted distance: %6.2f miles.\nCoordinates (weighted for population): %6.2f,%6.2f degrees.\nTotal weighted distance: %6.2f miles.\nExtra distance travelled: %6.2f miles. \n", gly, glx, total_distance, gly_w, glx_w, total_distance_w, distance_difference);
	//prints a message giving the nearest town/city if the weighted and non weighted ones are the same
	if (places[closestPosition][0] == places[closestPosition_w][0]){
		cout << "The nearest town/city to the non weighted co ordinates is the same as the weighted co ordinates and is " << places[closestPosition][0] << "." << endl;
	}
	//prints a message giving the nearest town/city if the weighted and non weighted ones are different
	else{
		cout << "The nearest city to the non weighted  co ordinates is " << places[closestPosition][0] << " whilst the nearest location for weighted co ordiantes is " << places[closestPosition_w][0] << ". \n" << endl;
	}
	return 0;
}
