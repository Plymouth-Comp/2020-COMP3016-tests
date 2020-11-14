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
    CHECK(reader.readFile("data/simples.dae") == false);
}

TEST_CASE("STD 12: Checking repeated Operations") {
    FileReader reader = FileReader();

    reader.openFile("data/simple.dae");
    CHECK(reader.readFile("data/simple.dae") == true);
    CHECK(reader.readFile("data/simple.dae") == true);
    reader.closeFile("data/simple.dae");
    CHECK(reader.currentFile().empty());
    CHECK(reader.getLines(0, 1).size() == 0);
}

TEST_CASE("STD 13: Multiple Files Can be Loaded and Inspected for their Elements") {
    FileReader reader = FileReader();

    //inspecting first file
    reader.openFile("data/simple.dae");
    reader.readFile("data/simple.dae");
    std::string value = reader.inspectForFirstAttribute("instance_geometry", "url");
    std::string comparison = "#Box_Mesh_345357280_002-mesh";
    CHECK(value.compare(comparison) == 0); //going beyond the length of the file on purpose
    value = reader.inspectForFirstAttribute("instance_geometry", "name");
    comparison = "Box_Mesh_345357280.002";
    CHECK(value.compare(comparison) == 0);


    //inspecting second file
    reader.openFile("data/advanced.dae");
    reader.readFile("data/advanced.dae");
    value = reader.inspectForFirstAttribute("geometry", "name");
    comparison = "Cube.12_1.001";
    CHECK(value.compare(comparison) == 0); 
    std::vector<std::string> lines = reader.inspectForFirst("geometry", "technique_common");
    REQUIRE(lines.size() > 1);
    comparison = "  <accessor source=\"#Cube_12_1_001-mesh-positions-array\" count=\"8\" stride=\"3\">";
    CHECK(lines[1].compare(comparison) == 0); 

    CHECK(reader.closeFile("data/simple.dae"));
    CHECK(reader.readFile("data/simple.dae") == false);
    CHECK(reader.currentFile().compare("data/advanced.dae") == 0);
    CHECK(reader.getLines(0, 10).size() == 11);
    reader.closeCurrentFile();
}

TEST_CASE("STD 14: Checking Large File") {
    FileReader reader = FileReader();

    REQUIRE(reader.openFile("data/large.dae"));
    CHECK(reader.readFile("data/large.dae") == true);
    std::string value = reader.inspectForFirstAttribute("instance_material", "target");
    std::string comparison = "#Leather_pouf-material";
    CHECK(value.compare(comparison) == 0);
    reader.closeCurrentFile();
}




TEST_CASE("ADV 1: Checking repeated Operations 2") {
    FileReader reader = FileReader();

    reader.openFile("data/simple.dae");
    CHECK(reader.readFile("data/simple.dae") == true);
    reader.closeFile("data/simple.dae");
    CHECK(reader.currentFile().empty());
    CHECK(reader.openFile("data/simple.dae") == true);
    CHECK(reader.openFile("data/simple.dae") == false);
    CHECK(reader.currentFile().compare("data/simple.dae") == 0);
    CHECK(reader.getLines(0, 187)[100].compare("            </index_of_refraction>") == 0);//FIXIT!
    CHECK(reader.closeCurrentFile());
    CHECK(reader.closeCurrentFile() == false);
}

TEST_CASE("ADV 2: Checking repeated Operations 3") {
    FileReader reader = FileReader();

    reader.openFile("data/simple.dae");
    CHECK(reader.openFile("data/advanced.dae") == true);
    CHECK(reader.currentFile().compare("data/advanced.dae") == 0);
    CHECK(reader.getLines(0, 1).size() > 0);
    CHECK(reader.closeCurrentFile());
    CHECK(reader.closeCurrentFile());
}

TEST_CASE("ADV 3: Checking repeated Operations 4") {
    FileReader reader = FileReader();

    reader.openFile("data/simple.dae");
    reader.openFile("data/advanced.dae");
    CHECK(reader.closeFile("data/simple.dae"));
    CHECK(reader.currentFile().compare("data/advanced.dae") == 0);
    CHECK(reader.getLines(0, 1).size() > 0);
    CHECK(reader.closeCurrentFile());
    CHECK(reader.currentFile().empty());
}

TEST_CASE("ADV 4: Checking repeated Operations 5") {
    FileReader reader = FileReader();

    reader.openFile("data/simple.dae");
    reader.openFile("data/advanced.dae");
    CHECK(reader.closeFile("data/advanced.dae"));
    CHECK(reader.currentFile().compare("data/simple.dae") == 0);
    CHECK(reader.getLines(0, 1).size() > 0);
    CHECK(reader.closeCurrentFile());
    CHECK(reader.currentFile().empty());
}


TEST_CASE("ADV 5: Trying to get a web-resource") {
    FileReader reader = FileReader();
    std::string fileName = "http://swen.fairrats.eu/research/webby.dae";
    CHECK(reader.openFile(fileName) == true);
    CHECK(reader.readFile(fileName) == true);
    std::vector<std::string> lines = reader.inspectForFirst("geometry", "mesh");
    REQUIRE(lines.size() > 1);
    std::string comparison = "  <source id=\"Creeper-mesh-positions\">";
    CHECK(lines[1].compare(comparison) == 0);
    CHECK(reader.closeCurrentFile());
}

TEST_CASE("ADV 6: Loading Irregular file") {
    FileReader reader = FileReader();
    std::string fileName = "http://swen.fairrats.eu/research/corrupt.dae";
    CHECK(reader.openFile(fileName) == true);
    CHECK(reader.getLines(0, 187)[49].compare("        </technique>") == 0);
    CHECK(reader.closeCurrentFile());
}


/*
* The specific content check in this method will be potentiall be
* updated a day before the deadline. But the calls will all stay the same.
*/
TEST_CASE("ADV 7: Trying to get a web-resource") {
    FileReader reader = FileReader();


    std::string urlFile = "http://swen.fairrats.eu/research/webby.dae";
    CHECK(reader.openFile(urlFile) == true);
    CHECK(reader.readFile(urlFile) == true);

    std::string value = reader.inspectForFirstAttribute("effect", "id");
    std::string comparison = "Material_004-effect";
    CHECK(value.compare(comparison) == 0); 
    value = reader.inspectForFirstAttribute("effect", "technique","sid");
    comparison = "common";
    CHECK(value.compare(comparison) == 0);
    value = reader.inspectForFirstAttribute("geometry", "source", "id");
    comparison = "Creeper-mesh-positions";
    CHECK(value.compare(comparison) == 0);


    //inspecting second file
    reader.openFile("data/advanced.dae");
    reader.readFile("data/advanced.dae");

    value = reader.inspectForFirstAttribute("geometry", "name");
    comparison = "Cube.12_1.001";
    CHECK(value.compare(comparison) == 0);
    std::vector<std::string> lines = reader.inspectForFirst("geometry", "technique_common");
    REQUIRE(lines.size() > 1);
    comparison = "  <accessor source=\"#Cube_12_1_001-mesh-positions-array\" count=\"8\" stride=\"3\">";
    CHECK(lines[1].compare(comparison) == 0);

    CHECK(reader.closeFile(urlFile));
    CHECK(reader.readFile(urlFile) == false);
    CHECK(reader.currentFile().compare("data/advanced.dae") == 0);

    CHECK(reader.getLines(0, 10).size() == 11);
    CHECK(reader.closeCurrentFile());
    CHECK(reader.currentFile().empty());
}
