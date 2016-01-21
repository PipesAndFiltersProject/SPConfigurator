//
//  StudentFileWriter.cpp
//  PipesAndFiltersFramework
//
//  Created by Antti Juustila on 30.1.2014.
//  Copyright (c) 2014 Antti Juustila. All rights reserved.
//

#include <chrono>

#include "StudentFileWriter.h"
#include "StudentDataItem.h"


namespace ohar_pipes {

	
	
StudentFileWriter::StudentFileWriter(const std::string & fileName) {
   using std::chrono::system_clock;
   file.open(fileName, std::ofstream::out | std::ofstream::app);
   if (file.is_open()) {
      system_clock::time_point today = system_clock::now();
      std::time_t tt;
      tt = system_clock::to_time_t ( today );
      file << std::endl << std::endl;
      file << "** Welcome to student register system" << std::endl;
      file << "** (c) Antti Juustila, 2014. University of Oulu, Finland." << std::endl;
      file << "** Today is: " << ctime(&tt) << std::endl;
      file << "** Following grades have been awarded:" << std::endl;
      file << "ID\tName\t\tDept\tExam\tExerc\tWork\tGRADE" << std::endl;
   }
}

StudentFileWriter::~StudentFileWriter() {
   file << std::endl << "**==--> End of batch <--==**" << std::endl;
   file.close();
}

void StudentFileWriter::write(const StudentDataItem * student) {
   if (file.is_open()) {
      file << *student;
   }
}


} //namespace