#include "../include/InvertedIndex.h"

std::vector<std::string>InvertedIndex::SplitIntoWords(const std::string &text) {
    std::vector<std::string> words;
    std::istringstream stream(text) ;//поток для разбиения строки
    std::string word ;
    while (stream >> word)
    {
        //удаляем знаки препинания и приводим к нижнему регистру
        word.erase(std::remove_if(word.begin(),word.end(),ispunct),word.end()) ;
        std::transform(word.begin(),word.end(),word.begin(),::tolower) ;
        words.push_back(word) ;
    }
    return words ;
}
//метод индексации одного документа
void InvertedIndex::IndexDocument(size_t doc_id, const std::string& doc_content) {
    std::unordered_map<std::string,size_t> word_freg_map ;//временный словарь для подсчёта слов
    std::vector<std::string> words = SplitIntoWords(doc_content) ;//разбиваем на слова
    //подсчитываем частоту каждого слова
    for(const auto& word : words)
    {
        word_freg_map[word]++ ;
    }
    std::lock_guard<std::mutex> lock(dictionary_mutex) ;
    //добавляем результаты в freq_dictionary
    for(const auto& [word,freg] : word_freg_map)
    {
        freq_dictionary[word].push_back(Entry{doc_id,freg}) ;
    }
}
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = input_docs ;//обновляем список документов
    freq_dictionary.clear() ;//очищаем старый частотный словарь

    std::vector<std::thread> threads ;//вектор потоков

    for(int doc_id = 0 ; doc_id < docs.size() ; ++doc_id)
    {
        threads.emplace_back(&InvertedIndex::IndexDocument,this,doc_id,docs[doc_id]) ;
    }
    //завершение всех потоков
    for(auto& thread : threads)
    {
        if(thread.joinable())
        {
            thread.join() ;
        }
    }
}
std::vector<Entry>InvertedIndex::GetWordCount(const std::string &word) {
    if(freq_dictionary.find(word) != freq_dictionary.end())
    {
        return freq_dictionary[word] ;//возвращаем список Antry для данного слова
    } else{
        return {} ;
    }
}
