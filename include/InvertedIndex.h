#ifndef SEARCH_ENGINE1_INVERTEDINDEX_H
#define SEARCH_ENGINE1_INVERTEDINDEX_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <thread>
#include <mutex>
#include <map>
#include <algorithm>

struct Entry
{
    size_t doc_id, count ;
    //Данный оператор необходим для проведения тестовых сценариев
    bool operator == (const Entry& other)const{
        return (doc_id == other.doc_id && count == other.count) ;
    }
};
class InvertedIndex
{
public:
    InvertedIndex() = default ;
    /**
     * Обновить или заполнить базу документов, по которой будем совершать
        поиск
    * @param texts_input содержимое документов
    */
    void UpdateDocumentBase(std::vector<std::string> input_docs);
    /**
    * Метод определяет количество вхождений слова word в загруженной базе
    документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    std::vector<Entry> GetWordCount(const std::string& word) ;
private:
    std::vector<std::string> docs ;//список содержимого документов
    std::map<std::string,std::vector<Entry>> freq_dictionary ;//частотный словарь
    std::mutex dictionary_mutex ;
    std::vector<std::string> SplitIntoWords(const std::string& text) ;//разбивает строку на слова
    /**
     * Индексирует один документ в отдельном потоке.
     * @param doc_id Идентификатор документа.
     * @param doc_content Содержимое документа.
     */
    void IndexDocument(size_t doc_id,const std::string& doc_content) ;
};

#endif //SEARCH_ENGINE1_INVERTEDINDEX_H
