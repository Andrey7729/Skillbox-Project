#ifndef SEARCH_ENGINE1_SEARCHSERVER_H
#define SEARCH_ENGINE1_SEARCHSERVER_H

#include "../include/InvertedIndex.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_set>

struct RelativeIndex//относительный индекс документа
{
    size_t doc_id ;
    float rank ;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};
class SearchServer
{
public:
/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
    SearchServer(InvertedIndex& idx) : _index(idx){ };
    /**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

private:
    InvertedIndex& _index;
    std::vector<std::string> SplitIntoWords(const std::string& query) ;//разбивает строку на слова
    std::vector<RelativeIndex> CalculateRelevance(const std::unordered_set<size_t>& relevant_docs,
                                                  const std::vector<std::string>& words) ;//релевантность документа
};

#endif //SEARCH_ENGINE1_SEARCHSERVER_H
