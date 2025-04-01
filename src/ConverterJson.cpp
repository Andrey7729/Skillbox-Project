#include "../include/ConverterJSON.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>

json ConverterJSON::loadConfig()  {
    std::fstream config_file("../config.json") ;
    if(!config_file.is_open())
    {
        throw std::runtime_error("config file is missing") ;
    }
    json config ;
    config_file >> config ;
    if(!config.contains("config"))
    {
        throw std::runtime_error("config file is empty") ;
    }

    std::string app_version = "0.1" ;
    if(config["config"]["version"].get<std::string>() != app_version)
    {
        throw std::runtime_error(" config.json has incorrect file version");
    }

    return config ;
}
std::vector<std::string> ConverterJSON::GetTextDocument() {
    std::vector<std::string> documents ;
    try {
        json config = loadConfig() ;
        const auto files = config["files"] ;
        for(const auto& file_path : files)
        {
            std::string path = file_path.get<std::string>() ;
            std::ifstream file(path) ;
            if(!file.is_open())
            {
                std::cerr << "Error: File " << path << " not found . Skipping..." << std::endl ;
                continue;
            }
            std::stringstream buffer ;
            buffer << file.rdbuf();//считываем весь файл в буфер

            documents.push_back(buffer.str()) ;
        }
    }catch (const std::exception& e)
    {
        throw ;
    }
    return documents ;
}
int ConverterJSON::GetResponsesLimit() {
    try{
        json config = loadConfig() ;
        if(config["config"].contains("max_responses"))
        {
            return config["config"]["max_responses"] ;
        } else{
            return 5 ; //значение по умолчанию
        }
    }catch (const std::exception& e)
    {
        throw ;
    }
}
std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> requests ;
    std::ifstream requests_file("../requests.json") ;
    if(!requests_file.is_open())
    {
        throw std::runtime_error("requests.json file is missing");
    }
    json requests_data ;
    requests_file >> requests_data ;
    if(!requests_data.contains("requests"))
    {
        throw std::runtime_error("requests.json has incorrect structure");
    }
    for(const auto& reg : requests_data["requests"])
    {
        requests.push_back(reg.get<std::string>()) ;
    }
    return requests ;
}
void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers){
    json answersJSON;//итоговый объект
    json request;//промежуточные данные о запросах

    for (size_t i = 0; i < answers.size(); i++) {
        std::string requestId = "request" + std::string(3 - std::to_string(i + 1).length(), '0') + std::to_string(i + 1);
        if (!answers[i].empty()) {
            request[requestId]["result"] = "true";

            json relevanceArray = json::array();//пустой массив для хранения документов и их рангов
            int responsesLimit = GetResponsesLimit();
            for (const auto& answer : answers[i]) {
                if (responsesLimit == 0) {
                    break;
                }
                responsesLimit--;

                json relevance;//для хранения инф о документе и его ранге
                relevance["docid"] = answer.first;
                relevance["rank"] = answer.second;
                relevanceArray.push_back(relevance);
            }
            request[requestId]["relevance"] = relevanceArray;
        } else {
            request[requestId]["result"] = "false";
        }
    }

    answersJSON["answers"] = request;

    std::ofstream file("../answers.json");
    if (!file.is_open()) {
        std::cerr << "Error opening answers.json for writing!" << std::endl;
        return;
    }
    std::cout << "Writing requests results to a file answers.json!" << std::endl;
    file << answersJSON.dump(4);
    file.close();
}

