#ifndef SEARCH_ENGINE1_CONVERTERJSON_H
#define SEARCH_ENGINE1_CONVERTERJSON_H

#include "nlohmann/json.hpp"
#include <vector>
#include <string>

using json = nlohmann::json ;

class ConverterJSON
{
public:
    ConverterJSON() = default ;
    //получить содержимое документов из config.json
    std::vector<std::string > GetTextDocument();
    //получить максимальное количество ответов
    int GetResponsesLimit();
    //получить список запросов из requests.json
    std::vector<std::string> GetRequests();
    //сохранить результаты поиска в answers.json
    void putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers);
private:
    //метод для загрузки и проверки config.json
    json loadConfig() ;
};

#endif //SEARCH_ENGINE1_CONVERTERJSON_H
