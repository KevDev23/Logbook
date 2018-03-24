/*
Author: Kevin Hinson
Name: LogBook
Compile: g++ LogBook.cpp
Description: takes records of activities and provides statistics
format in text files: activityName, startTime, endTime, totalTime(seconds/60/60)
first version for testing
*/
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<ctime>
#include<cstdlib>
#include<iterator>
#include<map>
#include<cstdio>
#include<iomanip>
#include<cstring>

struct set{//number of occurences and the actual relations
	int socc;//counts occurences of set
	std::vector<int> rel;//a relation in the set
};

struct day{//statistics on the day of the week for a activity
	int docc;//counts occurences of activity on corresponding day
	double dttime;//keeps total time of activity on corresponding day
	std::vector<set> dset;//sets in the corresponding day
};//consider a string var for day of the week

struct act{//statistics for the activity
	int actcount;//counts occurences of activity
	double attime;//total time spent on activity
	std::vector<set> overset;//over all set of hours spent on activity. EX: Tracks occurrences of hours 5-8 of activity across all days.
	std::vector<int> findrel;//finds relations for activity entry. 
	day week[7];
};
//===============================================================Function prototypes===============================================================
void nday(tm *date,std::string wday[],std::string months[], std::string daynum[],std::string &page,std::ofstream &bkpage,std::ofstream &mstfile,std::string &userin);
void write(std::ofstream &bkpage,std::string &page,std::string &userin,tm *date);
void eday(std::ofstream &bkpage,tm *date,std::string &page);
bool errnd(std::ofstream &mstfile,std::string &page);
int finds(std::vector<set> wholeset,std::vector<int> frels);
//void ini for new map entries
void stat(std::map<std::string,act> &data,act &temp,std::string wday[]);
void result(std::map<std::string,act> &data,std::string wday[]);
//====================================================	MAIN	=======================================================================================
int main(){
//VARIABLES
std::map<std::string,act> data;
act temp;
int num;//for translating string input into integer
bool serv = true;
//get day
	time_t curr;
	time(&curr);
    tm *date = localtime(&curr);//converts tm to localtime
//user input	
	std::string userin;//string of user input
	std::string act;//name of activity,is this actually necessary?
//Data for getting current day
	std::string wday[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};//week day
	std::string months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	std::string daynum[]= {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31"};//Couldn't trick a string to get int, so this line happened
	//may be getting dates wrong becuase 0 to n, whereas dates start at 1, added "0" to see if that would fix it.
	std:: string page;//will be current page
//For files	
	std::ofstream bkpage;//bkpage > bookpageopens a stream, go over this in detail, memory is sketchy
	std::ofstream mstfile;//for reading file names later
//getting date/pagename (Not sure if this is even necessary)
	std::string day = wday[date->tm_wday];//gets day
	std::string month = months[date->tm_mon];//gets month
	std::string dayof = daynum[date->tm_mday];//this gets wrong day now.
	page = day + month + dayof + ".txt";//creates name of txt file to hold records

while(serv)
{
	std::cout << "Logger MENU:\n";
	std::cout << "New Day(1)\n";
	std::cout << "New Entry(2)\n";
	std::cout << "End day(3)\n";
	std::cout << "Get statistics(4)\n";
	std::cout << "Quit(5)\n";

	std::cin >> userin;
num = atoi(userin.c_str());	
	switch(num)
	{
			case 1:
			nday(date,wday,months,daynum,page,bkpage,mstfile,userin);
			break;
			
			case 2:
			write(bkpage,page,userin,date);
			break;
			
			case 3:
			std::cout << "Writing end time to last activity...\n";
			eday(bkpage,date,page);
			break;
			
			case 4:
			std::cout << "Getting statistics...\n";
			stat(data,temp,wday);
			//result(data,wday);
			break;
			
			break;
			case 5:
			std::cout << "Quiting...goodbye!\n";
			serv = false;
			break;
			
			default:
			std::cout << "ERROR, refer to menu options numerically(example 4 means save and quit)\n";
			break;
	}
}
	

return 0;
}
///////////////////////////////////////////////		END MAIN	///////////////////////////////////////////////////////
//=====================================================Function Definitions=======================================================
void nday(tm *date,std::string wday[], std::string months[],std::string daynum[],std::string &page,std::ofstream &bkpage,std::ofstream &mstfile,std::string &userin)//pass streams as std::ofstream &bkpage
{

std::cout << "New Page started: " << page.c_str() << std::endl;

//bkpage.open(page.c_str());
mstfile.open("mstfile.txt",std::ofstream::app);
mstfile << page.c_str() << "\n";//records name of file in master list file
mstfile.close();

return;
}

/*
Name:write;
Writes activity, start & end time, and total time in seconds to files
*/
void write(std::ofstream &bkpage,std::string &page,std::string &userin,tm *date)
{
//vars
char buff[256];
int numlines = 0;//will contain number of lines
double totaltime;
time_t currs;//this will get time again
std::ifstream reader;//checks size of file
	/*
	if(errnd(mstfile,page))
	{
	  //call write?  
	}
	*/
time(&currs);//gets time
reader.open(page.c_str());

	while(reader.getline(buff,256)!=NULL)//gets number of lines to determine if the program is on it's first entry
	{
	 numlines++;
	}
	reader.close();

bkpage.open(page.c_str(),std::ofstream::app);
	if(numlines >= 1)//if this is not the first entry add end time
	{
	 totaltime = difftime(currs,mktime(date));
	 date = localtime(&currs);//gets time for var date, here it's end time
	 bkpage << date->tm_hour << " " << date->tm_min << " " << std::setprecision(3) << totaltime/60/60 << "\n";//record time for last activity
	}
std::cout << "What are you doing? >";
std::cin >> userin;
date = localtime(&currs);//gets time for var date, here it's start time
bkpage << userin << " " << date->tm_hour << " " << date->tm_min << " ";
bkpage.close();
return;
}

/*
Name:eday(end day)
Writes end time and total time to end of last entry, for use when ending a day.
*/
void eday(std::ofstream &bkpage,tm *date,std::string &page)
{
//vars
time_t currs;
double totaltime;
//---------------
time(&currs);
bkpage.open(page.c_str(),std::ofstream::app);

totaltime = difftime(currs,mktime(date));
date = localtime(&currs); 
bkpage << date->tm_hour << " " << date->tm_min << " " << std::setprecision(3) << totaltime/60/60 << "\n";
bkpage.close();
return;
}

/*
Name: errnd (error next day)
Description: error check for when attempting to write to a old page dated yesterday
returns: true if error, false if no error
*/
bool errnd(std::ofstream &mstfile,std::string &page)
{
//vars
char buff[256];//this will be compared to page later for a error check
FILE *icheck;//this will be used to read the last entry in the mstfile

icheck = fopen("mstfile.txt","r");
	while(fscanf(icheck,"%s",&buff)!=EOF)//gets to the end of file and puts the last entry in buff
	{
	  memset(buff,'\0',256);//resets buff to null for next entry until eof.
	}
fclose(icheck);

	if(strncmp(buff,page.c_str(),8)!=0)
	{
		std::cout << "Error, cannot write to a page made yesterday or already man\n";
		return true;
	}
	
return false;
}
/*
name: finds, as in find set
Description: Checks for wether or not a set already exists in the vector of sets to keep track of all sets
NOTE:This will expect overset to be passed as wholeset and findrel to frels(found relations).
*/
int finds(std::vector<set> wholeset,std::vector<int> frels)
{
	for(int i = 0; i < wholeset.size(); i++)
	{
		if(wholeset[i].rel==frels)
		{
			return i;//if the set is in the wholeset then return the index number of that set
		}
	}
	return -1;//if the set is not in wholeset then return false
}

/*
name:stat
description: This gets the statistics.
The variables denoted with a minute for future updates where more precision may be implemented
*/
void stat(std::map<std::string,act> &data,act &temp,std::string wday[])
{
//vars
int shour;//start hour
int smin;//start min
int ehour;//end hour
int emin;//end minutes
int giter = 0;//a iterator
int imp = 0;//temporary integer for general use(ex: imp = Legnth of some string)
set semp;//temporary set, socc, vector rel
std::map<std::string,act>::iterator iter;
std::vector<int>::iterator viter;//iterator for vectors
FILE *fmreader;//for reading the master file
FILE *bkreader;//for reading text files of log entries
std::string mstEntry;//master file entry
std::string key;//key for map(which is the activity name)
std::string kday;//day extracted from key
int index;//for the index nums in arrays
	fmreader = fopen("mstfile.txt","r");
	
		while(fscanf(fmreader,"%s",mstEntry.c_str()) != EOF )
		{
			bkreader = fopen(mstEntry.c_str(),"r");//open book pages(text files listed in the master file)
			
					while(fscanf(bkreader,"%s %d %d %d %d %d",key.c_str(),&shour,&smin,&ehour,&emin,&temp.attime) != EOF)
					{
						//build relation and find the day the activity occured on
								viter = temp.findrel.begin();//set vector iterator to beginning
							for(int i = shour; i <= ehour; i++)
							{
								temp.findrel.insert(viter,i);//might need to rely on resize
								viter++;
							}
								iter = data.find(key.c_str());//searches for key
								kday.insert(0,mstEntry,0,3);//kday.copy(mstEntry.c_str(),3,0);
							for(int i = 0; i < 7; i++)//finds which day of the week the current activity took place on
							{
								if(kday.compare(0,2,wday[i])==0)//strcmp(kday.c_str(),wday[i]) == 0)
									index = i;
							}
						//init new map entry, or append map entry	
						if(iter == data.end())//if there is no key then add it to the map, initialize data for new activity
						{	//set init values for the act overall, attime(activity total time) is read in the nested while loop
							semp.socc = 1;
							semp.rel = temp.findrel;
							temp.actcount = 1;
							temp.overset.insert(temp.overset.begin(),semp);
							//set init values for the day the activity occurred
							temp.week[index].docc = 1;
							temp.week[index].dttime = temp.attime;
							temp.week[index].dset.insert(temp.week[index].dset.begin(),semp); //insert semp, occurrences of a relation, and the relation.
							data.insert(std::pair<std::string,act>(key,temp));//insert temp into map creating the new map element
						}
						else{ //appending map entry
							  //getting overall totals
							  data[key].attime += temp.attime;
							  data[key].actcount++;
								imp = finds(data[key].overset,temp.findrel);
							  if(imp)//if imp finds the set
							  {
								data[key].overset[imp].socc++;
							  }
							  else
								 {
									semp.socc = 1;
									semp.rel = temp.findrel;
									data[key].overset.insert(data[key].overset.end(),semp);
							     }
							  //appending day activity occurred on					
							  data[key].week[index].dttime+=temp.attime;
							  data[key].week[index].docc++;
								imp = finds(data[key].week[index].dset,temp.findrel);
							  if(imp)//if finds finds the set in the day of the week
							  {
								data[key].week[index].dset[imp].socc++;
							  }
							  else{//insert new relation	
									semp.socc = 1;
									semp.rel = temp.findrel;
									data[key].week[index].dset.insert(data[key].week[index].dset.end(),semp);
								  }
							  
							}
					}
				fclose(bkreader);
		}
	fclose(fmreader);
	
return;
}

/*
name:result
param: map,wday
description: writes results of stat to a file
*/
void result(std::map<std::string,act> &data,std::string wday[])
{
std::string results;//name of the output file
int largest = -1;//temp that will represent the largest value
int lindex;//will take the index number of the element with the most value
int smallest = 999999;//temp that will represent the smallest value
int sindex;//will take the index number of the element with the smallest value
//FILE *fptr;
std::ofstream res;
	//getting name for out put file
	std::cout << "Enter a name for the statistics file>";
	 std::cin >> results;
	 std::cout << "\n";
	results += ".txt";
	
	res.open(results.c_str());
	//fptr = fopen(results.c_str(),"w");
	
		for(std::map<std::string,act>::iterator k = data.begin(); k != data.end(); k++)//maybe ++k
		{
			res << "Activity: " << k->first << "\n";
			res << "Total Time: " << data[k->first].attime << " hours, Average time: " << std::setprecision(3) << data[k->first].attime/data[k->first].actcount << " hours\n";
			res << "You did this activity: " << data[k->first].actcount << " times\n";
			//---------------------------------------------------------------------------------maybe make this as it's own function?
			for(int i = 0; i < data[k->first].overset.size(); i++)//finds max and min data values. For the most often and least often set of hours
			{
				if(data[k->first].overset[i].socc >= largest)
				{
					largest = data[k->first].overset[i].socc;
					lindex = i;
				}
				
				if(data[k->first].overset[i].socc <= smallest)
				{
					smallest = data[k->first].overset[i].socc;
					sindex = i;
				}
			}//------------------------------------------------
			largest = -1;
			res << "Most hours: ";
											for(int i = 0; i < data[k->first].overset[lindex].rel.size(); i++)
											{
												res << data[k->first].overset[lindex].rel[i] << " ";
											}
			res << "\nLeast hours: ";
											for(int i = 0; i < data[k->first].overset[sindex].rel.size(); i++)
											{
												res << data[k->first].overset[sindex].rel[i] << " ";
											}
			res << "\nDays:-----\n\t";
											for(int i = 0; i < 7; i++)
											{
												if(!data[k->first].week[i].dset.empty())
												{
													res << wday[i] << ": times done:" << data[k->first].week[i].docc << " Total hours on this day:" << data[k->first].week[i].dttime << " ";
													res << "Average hours: " << std::setprecision(3) << data[k->first].week[i].dttime/data[k->first].week[i].docc << " ";
												
													for(int j = 0; j < data[k->first].week[i].dset.size(); j++)//finds max and min data values. For the most often and least often set of hours
													{
														if(data[k->first].week[i].dset[j].socc >= largest)
														{
															largest = data[k->first].week[i].dset[j].socc;
															lindex = j;
														}
														
													}
													
													res << "Most often hours: ";
													for(int x  = 0; x < data[k->first].week[i].dset[lindex].rel.size(); x++)
													{
														res << data[k->first].week[i].dset[lindex].rel[x] << " ";
													}
													res << "\n-------------------------------------------------------\n";
												}
											}
			
			res << "==============================================================================================================================================\n";
		}
	res.close();
		
return;
}