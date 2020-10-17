/**
* DO NOT MODIFY! 
* Test File for the FileReader
* A final version will be relased close to the deadline with mroe detailed tests.
*/
#pragma once 
#include <string>
#include "../FileReader.h" 
//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"


TEST_CASE( "1: Creating a FileReader object"  ) {
    FileReader reader = FileReader();
    
    CHECK(reader.currentFile().empty());
}

TEST_CASE( "2: Opening File from data folder"  ) {
    FileReader reader = FileReader();

    CHECK( reader.openFile("data/simple.dae") == true );
}

TEST_CASE("3: Closing File from data folder" ) {
    FileReader reader = FileReader();

    CHECK(reader.closeFile("data/simple.dae") == true);
}

TEST_CASE("4: Closing currently open File" ) {
    FileReader reader = FileReader();

    CHECK(reader.closeCurrentFile() == true);
}

TEST_CASE("5: Opening and Reading a simple DAE File") {
    FileReader reader = FileReader();
    std::string fileName = "data/simple.dae";
    CHECK(reader.openFile(fileName) == true);
    std::string checkName = reader.currentFile();
  
    CHECK(checkName.compare(checkName.size() - fileName.size(), fileName.size(), fileName) == 0);
    std::vector<std::string> lines = reader.getLines(0, 187);
    
    CHECK(lines.size() == 187);
    reader.closeCurrentFile();
}

TEST_CASE("6: Opening and Reading a large DAE File and trying to load too many lines.") {
    FileReader reader = FileReader();
    std::string fileName = "data/large.dae";
    CHECK(reader.openFile(fileName) == true);
    std::string checkName = reader.currentFile();

    CHECK(checkName.compare(checkName.size() - fileName.size(), fileName.size(), fileName) == 0);
    CHECK(reader.getLines(0, 300).size() > 0); //going beyond the length of the file on purpose
    std::vector<std::string> lines = reader.getLines(0, 300);

    CHECK(lines.size() == 202);
    reader.closeCurrentFile();
}

TEST_CASE("7: Trying to get a node from a simple dae file") {
    FileReader reader = FileReader();
    std::string fileName = "data/simple.dae";
    CHECK(reader.openFile(fileName) == true);
    
    std::vector<std::string> lines = reader.inspectForFirst("geometry");
    std::string comparison = "<geometry id = \"Box_Mesh_345357280_002-mesh\" name = \"Box_Mesh_345357280.002\">";
    CHECK(lines[0].compare(comparison)==0); //going beyond the length of the file on purpose
    comparison = "</source>";
    CHECK(lines[30].compare(comparison) == 0);
    reader.closeCurrentFile();
}

TEST_CASE("8: Trying to get a node and subnode from a simple dae file") {
    FileReader reader = FileReader();
    std::string fileName = "data/simple.dae";
    CHECK(reader.openFile(fileName) == true);

    std::vector<std::string> lines = reader.inspectForFirst("geometry","triangles");
    std::string comparison = "<geometry id = \"Box_Mesh_345357280_002-mesh\" name = \"Box_Mesh_345357280.002\">";
    CHECK(lines[0].compare(comparison) == 0); //going beyond the length of the file on purpose
    comparison = "</source>";
    CHECK(lines[30].compare(comparison) == 0);
    reader.closeCurrentFile();
}


TEST_CASE("9: Trying to get a node attribute from a simple dae file") {
    FileReader reader = FileReader();
    std::string fileName = "data/simple.dae";
    CHECK(reader.openFile(fileName) == true);

    std::string value = reader.inspectForFirstAttribute("geometry", "name");
    std::string comparison = "Box_Mesh_345357280.002";
    CHECK(value.compare(comparison) == 0); //going beyond the length of the file on purpose
    value = reader.inspectForFirstAttribute("geometry", "id");
    comparison = "Box_Mesh_345357280_002-mesh";
    CHECK(value.compare(comparison) == 0);
    reader.closeCurrentFile();
}

TEST_CASE("10: Trying to get a node attribute from a simple dae file") {
    FileReader reader = FileReader();
    std::string fileName = "data/simple.dae";
    CHECK(reader.openFile(fileName) == true);

    std::string value = reader.inspectForFirstAttribute("geometry", "name");
    std::string comparison = "Box_Mesh_345357280.002";
    CHECK(value.compare(comparison) == 0); //going beyond the length of the file on purpose
    value = reader.inspectForFirstAttribute("geometry", "id");
    comparison = "Box_Mesh_345357280_002-mesh";
    CHECK(value.compare(comparison) == 0);
    reader.closeCurrentFile();
}//

