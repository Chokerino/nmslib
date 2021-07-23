/**
 * Non-metric Space Library
 *
 * Main developers: Bilegsaikhan Naidan, Leonid Boytsov, Yury Malkov, Ben Frederickson, David Novak
 *
 * For the complete list of contributors and further details see:
 * https://github.com/nmslib/nmslib
 *
 * Copyright (c) 2013-2018
 *
 * This code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 *
 */
#ifndef _SPACE_FASTANI_
#define _SPACE_FASTANI_

#include <string>
#include <map>
#include <stdexcept>
#include <algorithm>

#include <string.h>
#include "distcomp.h"
#include "space/space_string.h"
#include "cgi/calDist.hpp"

namespace similarity {

#define length(array) ((sizeof(array)))

#define FASTANI       "fastani"
#define FASTAAI       "fastaai"

class SpaceFastANI : public StringSpace<float> {
 public:
  explicit SpaceFastANI() {}
  virtual ~SpaceFastANI() {}
  virtual std::string StrDesc() const { return "FastANI distance"; }

 protected:
  virtual float HiddenDistance(const Object* obj1, const Object* obj2) const {
    CHECK(obj1->datalength() > 0);
    CHECK(obj2->datalength() > 0);
    string x_temp = CreateStrFromObj(obj1, "");
    string y_temp = CreateStrFromObj(obj2, "");
    char* x = (char *)x_temp.c_str();
    char* y = (char *)y_temp.c_str();
    //std::cout<< "String: " << x << " " << y << " " <<std::endl;
    char* command[] = {"./fastANI", "-q", x, "-r" , y};
    float result = dist::getDistance(command);
    return 100.0 - result;
  }
  DISABLE_COPY_AND_ASSIGN(SpaceFastANI);
};

class SpaceFastAAI : public StringSpace<float> {
 public:
  explicit SpaceFastAAI() {}
  virtual ~SpaceFastAAI() {}
  virtual std::string StrDesc() const { return "FastAAI distance"; }

 protected:
  virtual float HiddenDistance(const Object* obj1, const Object* obj2) const {
    CHECK(obj1->datalength() > 0);
    CHECK(obj2->datalength() > 0);
    string filename = ".aai_filenames.txt";
    ofstream outputfile;
    string x_temp = CreateStrFromObj(obj1, "");
    string y_temp = CreateStrFromObj(obj2, "");
    char* x = (char *)x_temp.c_str();
    char* y = (char *)y_temp.c_str();

    outputfile.open(filename);
    outputfile << x << "\n" << y;
    outputfile.close();

    system("comparem aai_wf aai_filenames.txt aai_outputs");
    const char * AAI_File = "aai_outputs/aai/aai_summary.tsv";
    ifstream  fileP(AAI_File);

    string labels, genomeA, genesA, genomeB, genesB, orthogenes, aai;
    getline(fileP, labels, '\n'); //Header File

    std::getline(fileP, genomeA, '\t'); //Genome A
    std::getline(fileP, genesA, '\t'); //Genes in A
    std::getline(fileP, genomeB, '\t'); //Genome B
    std::getline(fileP, genesB, '\t'); //Genes in B
    std::getline(fileP, orthogenes, '\t'); //Orthogonal Genes
    std::getline(fileP, aai, '\t'); //AAI Score

    double result = ::atof(aai.c_str());
    system("rm -rf aai_outputs/");
    system("rm -f .aai_filenames.txt");
    return 100.0 - result;
  }
  DISABLE_COPY_AND_ASSIGN(SpaceFastAAI);
};
}  // namespace similarity

#endif
