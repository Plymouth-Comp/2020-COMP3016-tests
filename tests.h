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


TEST_CASE( "STD 1: Creating a FileReader object"  ) {
    FileReader reader = FileReader();
    
    CHECK(reader.currentFile().empty());
}

TEST_CASE( "STD 2: Opening File from data folder"  ) {
    FileReader reader = FileReader();

    CHECK( reader.openFile("data/simple.dae") == true );
}

TEST_CASE("STD 3: Closing File from data folder" ) {
    FileReader reader = FileReader();
    reader.openFile("data/simple.dae");
    CHECK(reader.closeFile("data/simple.dae") == true);
}

TEST_CASE("STD 4: Closing currently open File" ) {
    FileReader reader = FileReader();
    reader.openFile("data/simple.dae");
    CHECK(reader.closeCurrentFile() == true);
}

TEST_CASE("STD 5: Opening and Reading a simple DAE File") {
    FileReader reader = FileReader();
    std::string fileName = "data/simple.dae";
    reader.openFile(fileName);
    std::string checkName = reader.currentFile();
  
    CHECK(checkName.compare(checkName.size() - fileName.size(), fileName.size(), fileName) == 0);
    std::vector<std::string> lines = reader.getLines(0, 187);
    
    CHECK(lines.size() == 188);
    reader.closeCurrentFile();
}

TEST_CASE("STD 6: Opening and Reading a large DAE File and trying to load too many lines.") {
    FileReader reader = FileReader();
    std::string fileName = "data/large.dae";
    CHECK(reader.openFile(fileName) == true);
    std::string checkName = reader.currentFile();

    CHECK(checkName.compare(checkName.size() - fileName.size(), fileName.size(), fileName) == 0);
    CHECK(reader.getLines(0, 300).size() > 0); //going beyond the length of the file on purpose
    std::vector<std::string> lines = reader.getLines(0, 300);

    CHECK(lines.size() == 203);
    reader.closeCurrentFile();
}

TEST_CASE("STD 7: Trying to get a node from a simple dae file") {
    FileReader reader = FileReader();
    std::string fileName = "data/simple.dae";
    CHECK(reader.openFile(fileName) == true);
    
    std::vector<std::string> lines = reader.inspectForFirst("geometry");
    std::string comparison = "<geometry id=\"Box_Mesh_345357280_002-mesh\" name=\"Box_Mesh_345357280.002\">";
    REQUIRE(lines.size() > 1);
    CHECK(lines[0].compare(comparison)==0); //going beyond the length of the file on purpose
    comparison = "    </source>";
    CHECK(lines[30].compare(comparison) == 0);
    reader.closeCurrentFile();
}

TEST_CASE("STD 8: Trying to get a node and subnode from a simple dae file") {
    FileReader reader = FileReader();
    std::string fileName = "data/simple.dae";
    CHECK(reader.openFile(fileName) == true);

    std::vector<std::string> lines = reader.inspectForFirst("geometry","triangles");
    REQUIRE(lines.size() > 1);
    std::string comparison = "<triangles material=\"Material_004-material\" count=\"12\">";
    CHECK(lines[0].compare(comparison) == 0); //going beyond the length of the file on purpose
    comparison = "  <input semantic=\"NORMAL\" source=\"#Box_Mesh_345357280_002-mesh-normals\" offset=\"1\"/>";
    CHECK(lines[2].compare(comparison) == 0);
    reader.closeCurrentFile();
}


TEST_CASE("STD 9: Trying to get a node attribute from a simple dae file") {
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


TEST_CASE("STD 10: Trying to get a node attribute from a simple dae file") {
    FileReader reader = FileReader();
    std::string fileName = "data/simple.dae";
    CHECK(reader.openFile(fileName) == true);

    std::string value = reader.inspectForFirstAttribute("instance_geometry", "url");
    std::string comparison = "#Box_Mesh_345357280_002-mesh";
    CHECK(value.compare(comparison) == 0); //going beyond the length of the file on purpose
    value = reader.inspectForFirstAttribute("instance_geometry", "name");
    comparison = "Box_Mesh_345357280.002";
    CHECK(value.compare(comparison) == 0);
    reader.closeCurrentFile();
}

TEST_CASE("STD 11: Opening an invalid File from data folder") {
    FileReader reader = FileReader();
    //wrong file name supplied
    CHECK(reader.openFile("data/simples.dae") == false);
}

TEST_CASE("STD 12: Checking repeated Operations") {
    FileReader reader = FileReader();

    reader.openFile("data/simple.dae");
    CHECK(reader.readFile("data/simple.dae") == true);
    reader.closeFile("data/simple.dae");
    CHECK(reader.currentFile().empty());
    CHECK(reader.getLines(0, 1).size() == 0);
}

TEST_CASE("ADV 1: Trying to get a web-resource") {
    FileReader reader = FileReader();
    std::string fileName = "https://swen.fairrats.eu/research/webby.dae";
    CHECK(reader.openFile(fileName) == true);
}
