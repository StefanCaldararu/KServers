//TODO: Create object for file reader
    std::fstream fin;
    fin.open(inputFile, std::ios::in);
    //TODO: create object for line parser, line, word, row
    std::vector<std::string> row;
    std::string line, word;
    //The first line is the number of mspaces
    getline(fin, line);
    int num_mspaces = std::stoi(line);
    //now we know number of mspaces, can go through each one and add the inputs in.
    for(int i = 0;i<num_mspaces;i++){
        //now we need to read in the mspace.
        //first, size of mspace:
        getline(fin, line);
        int size = std::stoi(line);
        //FIXME: might have problems with destructor inside of for loop...
        Mspace temp(size);
        spaces.push_back(temp);
        //fill in the mpsace row by row, each row is on a new line (hopefully)
        //TODO: make sure that the mspace is in this format, and comment at top reflects this.
        for(int j = 0; j<size; j++){
            getline(fin, line);
            //Fill in this row
            std::stringstream str(line);
            int k = 0;
            while(getline(str, word, ',')){
                spaces[i].setDistance(j, k, stoi(word));
                k++;
            }
        }
        //now the mspace is filled in correctly. we now need to make sure that 
        //the inputs all get filled in.
        getline(fin, line);
        int NI = std::stoi(line);
        num_inputs.push_back(NI);
        for(int j = 0; j<NI; j++){
            std::vector<int> input;
            getline(fin, line);
            std::stringstream str(line);
            while(getline(str, word, ','))
                input.push_back(stoi(word));
            inputs.push_back(input);
        }
        //Now all of the inputs have been put in the correct places, we can now start again with the
        //next metric space.
    }
    
    return num_mspaces;