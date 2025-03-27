#include "../include/SearchServer.h"
#include <sstream>
#include <cmath>
#include <numeric>

std::vector<std::string>SearchServer::SplitIntoWords(const std::string &query) {
    std::vector<std::string> words;
    std::istringstream stream(query) ;//поток для разбиения строки
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
//функция расчёта релевантности
std::vector<RelativeIndex>SearchServer::CalculateRelevance(const std::unordered_set<size_t> &relevant_docs,
                                                           const std::vector<std::string> &words) {
    std::vector<RelativeIndex> ranks ;
    std::unordered_map<size_t,float> absolute_relevance ;

    for(const auto& doc_id : relevant_docs)
    {
        float abs_relevance = 0 ;
        for(const auto& word : words)
        {
            auto count_vector = _index.GetWordCount(word) ;
            for(const auto& entry : count_vector)
            {
                if(entry.doc_id == doc_id)
                {
                    abs_relevance += entry.count ;
                }
            }
        }
        absolute_relevance[doc_id] = abs_relevance ;
        ranks.push_back({doc_id,abs_relevance}) ;
    }
    float max_relevance = 0 ;
    for(const auto& rank : ranks)
    {
        if(absolute_relevance[rank.doc_id] > max_relevance)
        {
            max_relevance = absolute_relevance[rank.doc_id] ;
        }
    }
    for(auto& rank : ranks)
    {
        rank.rank = max_relevance > 0 ? rank.rank/ max_relevance : 0.0f ;
    }
    std::sort(ranks.begin(),ranks.end(),[](const RelativeIndex& a,const RelativeIndex& b){
        return a.rank > b.rank ;
    });
    return ranks ;
}
//метод обработки поисковых запросов
std::vector<std::vector<RelativeIndex>>SearchServer::search(const std::vector<std::string> &queries_input) {
    std::vector<std::vector<RelativeIndex>> results;

    for(const auto& query : queries_input)
    {
        auto words = SplitIntoWords(query) ;
        std::map<std::string ,size_t> word_frequency ;

        for(const auto& word : words)
        {
            auto count_vector = _index.GetWordCount(word) ;
            word_frequency[word] = count_vector.size() ;//частота слов
        }
        std::sort(words.begin(),words.end(),[&word_frequency](const std::string& a,const std::string& b){
            return word_frequency[a] < word_frequency[b] ;
        });
        std::unordered_set<size_t> relevant_docs;

        for (const auto& w : words) {
            auto count_vector = _index.GetWordCount(w);
            std::unordered_set<size_t> current_docs;
            for (const auto& entry : count_vector) {
                current_docs.insert(entry.doc_id);
            }

            if (relevant_docs.empty()) {
                relevant_docs = std::move(current_docs);
            } else {
                std::unordered_set<size_t> temp_docs;
                for (const auto& doc_id : current_docs) {
                    if (relevant_docs.find(doc_id) != relevant_docs.end()) {
                        temp_docs.insert(doc_id);
                    }
                }
                relevant_docs = std::move(temp_docs);
            }
        }

        if (relevant_docs.empty()) {
            results.push_back({});
            continue;
        }

        results.push_back(CalculateRelevance(relevant_docs, words));
    }

    return results;
}
