#include <iostream>

// quandl_first_program.cpp
// PLACE_YOUR_QUANDL_TOKEN_HERE in the main() function
// should be replaced by your Quandl authorization token.
// compile with
// c++ -o q_f_p quandl_first_program.cpp -lcurl -std=c++11
// execute with
// ./q_f_p
#include <ctype.h>
#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>
#include <vector>
#include <set>
using std::ifstream;
#include <map>

typedef long int myint;
void printToFile(std::string filename, const std::string &text, std::string ticker){
    int count=0;
    std::ofstream mfile;
    mfile.open(filename);
    //mfile <<ticker;
    //mfile <<std::endl;
    for(myint i=text.find("data",300)+9;i<text.find("order");++i){
      if ( ( isdigit(text[i])) or (text[i]=='.' ) or (text[i]==',' )  ){
        if ( isdigit(text[i]) or (text[i]=='.' ) ) {mfile<<text[i];}
        if (text[i]==','){ mfile<<std::endl; ++count;}
    }
  }
    mfile.close();
  //  std::cout<< "this is number of lines"<< count <<std::endl;
}
static size_t writerF(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    ((std::string*)userdata)->append((char*)ptr, size * nmemb);
    return size * nmemb;
}
void stockDataToFile(const std::string &tickerName,
                     const std::string &quandl_auth_token,
                     const std::string &database="WIKI",
                     const std::string &folder="./quotes/"){
    std::string mainLink="https://www.quandl.com/api/v3/datasets/FSE/";
  //  mainLink+=database;
    mainLink+="/"+tickerName +"?";
    mainLink+="column_index=4&start_date=2018-11-03&end_date=2018-12-05";
    mainLink+=quandl_auth_token;
    CURL *curl;
    std::string quandlData;
    std::string fName=folder;
    fName+=tickerName;
    fName+=".txt";
    curl = curl_easy_init();
    if(curl) {
        const char* linkArrayChar=mainLink.c_str();
        curl_easy_setopt(curl, CURLOPT_URL, linkArrayChar);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writerF);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &quandlData);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        printToFile(fName,quandlData, tickerName);
    }
}
int main(){
    std::vector<double> v;
    std::set <std::vector<double> > all_thirty_day;
    std::set <std::vector<double> >::iterator vt;
    std::vector <double>::iterator it;
    v.resize(51); // will not contain more than 30 days worth of data
//    std::set<double>::iterator it;
    std::string line;
    std::string filename;
    std::cout<< "please enter the stock that you would like to add to the portfolio"<< std::endl;
    std::string str;
    while (str!="stop"){
    std::cin>> str;
    if ( str!="stop"){
    std::string myQuandlToken="&api_key=3Gsxr4DhNcTwpSBqkVm8";
    std::string stockName=str;
    stockDataToFile(stockName,myQuandlToken);
    // starting here 30 day data put in vector
    int i=0; int j=0; // j will be used as position in vector
    // we change the directory to open the file
    filename= "./quotes/"+str+".txt";
    ifstream myfile(filename);
     if (!myfile){ std::cout<<"cannot open input file"<<std::endl;}
     else{ while(getline(myfile,line)){
            // reading from the file and puting close price in a vector
            if (( i%2!=0) and (std::stof(line)!=0)){
             v[j]=std::stof(line);
             j++;}
            i++;}}
     //taking the zeros out from th e vectors
     v.resize(j);
     all_thirty_day.insert(v);

 }
}
// to access each element of each stock to calculate return and covariance
  for (vt=all_thirty_day.begin(); vt!=all_thirty_day.end(); ++vt){
    std::cout << "printing a new stock return" << std::endl;
      int siz= ((*vt)).size();

     for ( int i=0; i< siz; i++){
       std::cout << ((*vt))[i] << std::endl; // look here to calculate the return each of these is the close price that is being printed
     }


  }




    return 0;
}
