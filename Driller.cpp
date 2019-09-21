#include <iostream>
#include <string> 
#include <vector> 
#include <sstream>
#include <fstream> 
#include <iomanip>
using namespace std;

struct drillingRecord
{
	double nums[16]; // holds the numeric data, in column order
	string strings[2]; // holds the string data, in column order
};

struct drillingArray
{
	int capacity; // maximum capacity, in records
	drillingRecord* data = NULL; // pointer to array of records
};

//DrillingArray Function
drillingArray* doubleDrillingArray(drillingArray* currentDrillingArray)
{
	int currentDrillingCapacity = currentDrillingArray->capacity;
	drillingArray* newDrillingArray = new drillingArray;
	newDrillingArray->data = new drillingRecord[currentDrillingArray->capacity * 2];
	newDrillingArray->capacity = currentDrillingCapacity * 2;

	for (int index = 0; index < currentDrillingCapacity; ++index)
	{
		newDrillingArray->data[index] = currentDrillingArray->data[index];
	}
	delete[] currentDrillingArray->data;
	delete currentDrillingArray;

	return newDrillingArray;
}

int main()
{
	//Variables for the program
	int index1 = 0;
	int index2 = 0;
	string line1;
	string line2;
	string date;
	vector<int> lineArray;
	vector<string> skipLineArray;
	vector<string> dataArray;
	vector<string> dateArray;
	vector<string> timeArray;
	vector<string> valueArray;
	vector<string> resultLine2;
	vector<string> errorMessage;
	vector<string> resultLine;

	getline(cin, line1);

	//Read in the file and add to array
	while (getline(cin, line1))
	{
		string str;
		istringstream inside(line1);
		while (getline(inside, str, ','))
		{
			dataArray.push_back(str);
		}
		dateArray.push_back(dataArray[index1]);
		timeArray.push_back(dataArray[index1 + 1]);
		date = dateArray[0];
		index1 += 18;
	}

	//Code for checking date if it is matching
	long arraySize = dataArray.size();
	long dateSize = dateArray.size();
	for (int index = 2; index < arraySize; ++index)
	{
		for (int cont = 0; cont < 16; ++cont)
		{
			valueArray.push_back(dataArray[cont + index]);
		}
		index += 17;
	}

	for (int index = 0; index < dateSize; ++index)
	{
		line2 = "";
		if (date == dateArray[index])
		{
			for (int counter = 0; counter <= 17; ++counter)
			{
				line2.append(dataArray[counter + index2]);
				if (counter < 17)
				{
					line2.append(";");
				}
			}
			skipLineArray.push_back(line2);
			index2 += 18;
		}
		else
		{
			int lineCounter = 16 * index;
			skipLineArray.push_back("Dates are not matching " + dateArray[index] + " at line " + to_string(index + 1) + ".");
			timeArray[index] = to_string(index);

			for (int count = 0; count < 16; ++count)
			{
				valueArray[(lineCounter + count)] = to_string(1);
			}
			index2 += 18;
			lineArray.push_back(index + 1);
		}
	}

	//Code for checking time if it is matching
	long timeSize = timeArray.size();
	for (int index1 = 0; index1 < timeSize; ++index1)
	{
		string str = timeArray[index1];
		for (int perCol = 1; perCol < timeSize; ++perCol)
		{
			if ((index1 + perCol) < timeSize)
			{
				if (str == timeArray[perCol + index1])
				{
					skipLineArray[index1 + perCol] = "Duplicate timestamp " + timeArray[index1 + perCol] + " at line " + to_string(index1 + perCol + 1) + ".";

					int lineCounter = 16 *(perCol + index1);
					
					for (int count = 0; count < 16; ++count)
					{
						valueArray[(lineCounter + count)] = to_string(1);
					}
					lineArray.push_back(index1 + perCol + 1);
				}
			}
		}
	}

	//Code for checking value if it is matching
	long valueSize = valueArray.size();
	int value = 0;
	int lineCounter = 1;
	for (int index = 0; index < valueSize; ++index)
	{
		if ((index - 16) == value)
		{
			++lineCounter;
			value = index;
		}

		double first = stof(valueArray[index].c_str());
		if (first <= 0)
		{
			skipLineArray[lineCounter - 1] = "Invalid floating-point data at line " + to_string(lineCounter) + ".";
			lineArray.push_back(lineCounter);
		}
	}

	//Code for adding it to the lineArray
	long lineArraySize = lineArray.size();
	long resultLineSize = skipLineArray.size();
	for (int index1 = 0; index1 < lineArraySize; ++index1)
	{
		for (int index2 = 1 + index1; index2 < lineArraySize; ++index2)
		{
			if (lineArray[index1] > lineArray[index2])
			{
				int temp = 0;
				temp = lineArray[index1];
				lineArray[index1] = lineArray[index2];
				lineArray[index2] = temp;
			}
		}
	}

	//Code for error message 
	for (int index = 0; index < lineArraySize; ++index)
	{
		string temp = skipLineArray[lineArray[index] - 1];
		errorMessage.push_back(temp);
		skipLineArray[lineArray[index] - 1].erase();
	}

	//Code for reading in the line
	for (int index = 0; index < resultLineSize; ++index)
	{
		string subString;
		istringstream line(skipLineArray[index]);

		while (getline(line, subString, ';'))
		{
			resultLine.push_back(subString);
		}
	}

	//Pointing for the drilling array
	drillingArray* drillArray = new drillingArray;
	drillArray->data = new drillingRecord[10];
	drillArray->capacity = 10;

	//Code for increasing the size of the array
	long numCount = (resultLine.size() / 18);
	int index = 0;
	while (index < numCount)
	{
		if (numCount <= drillArray->capacity)
		{
			int lineCount = index * 18;
			drillArray->data[index].strings[0] = resultLine[lineCount];
			drillArray->data[index].strings[1] = resultLine[lineCount + 1];

			for (int count = 0; count < 16; ++count)
			{
				double d = atof(resultLine[lineCount + count + 2].c_str());
				drillArray->data[index].nums[count] = d;
			}
			++index;
		}
		else
		{
			drillArray = doubleDrillingArray(drillArray);
		}
	}

	//Code for adding the the elements to the array
	for (int index = 0; index < numCount / 2; ++index)
	{
		drillingRecord* temp = new drillingRecord;
		*temp = drillArray->data[index];
		drillArray->data[index] = drillArray->data[(numCount - 1) - index];
		drillArray->data[(numCount - 1) - index] = *temp;
	}

	//Code for error message
	long errorMessageSize = errorMessage.size();
	for (int index = 0; index < errorMessageSize; ++index)
	{
		cout << errorMessage[index];
		if (errorMessage[index] != "")
		{
			cout << endl;
		}
	}

	//Code for printing out the array
	for (int index = 0; index < numCount; ++index)
	{
		cout << drillArray->data[index].strings[0] << ";";
		cout << drillArray->data[index].strings[1] << ";";
		for (int count = 0; count < 16; ++count)
		{
			cout << fixed << setprecision(2) << drillArray->data[index].nums[count];
			if (count < 15)
			{
				cout << ";";
			}
		}
		cout << endl;
	}
	return 0;
}