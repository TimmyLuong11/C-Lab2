#include <iostream>
#include <string> 
#include <vector> 
#include <sstream>
#include <fstream> 
#include <iomanip>
#include "Driller.h"
using namespace std;

struct drillingRecord
{
	double nums[16]; // holds the numeric data, in column order
	string strings[2]; // holds the string data, in column order
};struct drillingArray 
{
	int capacity; // maximum capacity, in records
	drillingRecord* data = NULL; // pointer to array of records
};drillingArray* doubleDrillingArray(drillingArray* currentDrillingArray)
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
		}
	}

	//Code for checking time if it is matching
	long timeSize = timeArray.size();
	for (int index = 0; index < timeSize; ++index)
	{
		string str = timeArray[index];
		for (int perCol = 1; perCol < timeSize; ++perCol)
		{
			if ((index + perCol) < timeSize)
			{
				if (str == timeArray[perCol + index])
				{
					skipLineArray[index + perCol] = "Duplicate time stamp " + timeArray[index + perCol] + " at line " + to_string(index + perCol + 1) + ".";

					int lineCounter = 16 * perCol;

					for (int count = 0; count < 16; ++count)
					{
						valueArray[(lineCounter + count)] = to_string(1);
					}
				}
			}
		}
	}

	//Code for checking value if it is matching
	long valueSize = valueArray.size();
	long skiplineSize = skipLineArray.size();
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
		}
	}

	for (int index = 0; index < skiplineSize; ++index)
	{
		cout << skipLineArray[index] << endl;
	}


	vector<string> resultLine2;
	vector<string> errorMessage;
	vector<string> resultLine;
	long lineArraySize = lineArray.size();
	long resultLineSize = skipLineArray.size();


	for (int index = 0; index < lineArraySize; ++index) 
	{
		for (int i = 1 + index; i < lineArraySize; ++i) 
		{
			if (lineArray[index] > lineArray[i])
			{
				int temp = 0;
				temp = lineArray[index];
				lineArray[index] = lineArray[i];
				lineArray[i] = temp;
			}
		}
	}

	for (int index = 0; index < lineArraySize; ++index) 
	{
		string temp = skipLineArray[lineArray[index] - 1];
		errorMessage.push_back(temp);
		skipLineArray[lineArray[index] - 1].erase();
	}

	for (int index = 0; index < resultLineSize; ++index) 
	{
		string subString;
		istringstream line(skipLineArray[index]);

		while (getline(line, subString, ';')) 
		{
			resultLine.push_back(subString);
		}
	}


	drillingArray* drillArray = new drillingArray;
	drillArray->data = new drillingRecord[10];
	drillArray->capacity = 10;
	long numCount = (resultLine.size() / 18);

	for (int index = 0; index < numCount;) 
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

	for (int index = 0; index < numCount / 2; ++index)
	{
		drillingRecord* temp = new drillingRecord;
		*temp = drillArray->data[index];
		drillArray->data[index] = drillArray->data[(numCount - 1) - index];
		drillArray->data[(numCount - 1) - index] = *temp;
	}

	long errorMessageSize = errorMessage.size();

	for (int index = 0; index < errorMessageSize; ++index) 
	{
		cout << errorMessage[index];
		if (errorMessage[index] != "") 
		{
			cout << endl;
		}
	}

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