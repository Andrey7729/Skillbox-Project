#include <iostream>
#include "../include/ConverterJSON.h"
#include "../include/InvertedIndex.h"
#include "../include/SearchServer.h"
#include <vector>

int main() {
    ConverterJSON converterJson ;
    InvertedIndex index ;

    std::vector<std::string> textDocuments = converterJson.GetTextDocument() ;

    index.UpdateDocumentBase(textDocuments) ;

    SearchServer server(index) ;

    auto relevantAnswers = server.search(converterJson.GetRequests()) ;

    // Преобразование std::vector<std::vector<RelativeIndex>> в std::vector<std::vector<std::pair<int, float>>>
    std::vector<std::vector<std::pair<int,float>>> convertedAnswers ;
    for(const auto& outerVec : relevantAnswers)
    {
        std::vector<std::pair<int,float>> convertedInnerVec ;
        for(const auto& relativeIndex : outerVec)
        {
            convertedInnerVec.emplace_back(relativeIndex.doc_id, relativeIndex.rank);
        }
        convertedAnswers.emplace_back(convertedInnerVec);
    }
    converterJson.putAnswers(convertedAnswers) ;

    return 0;
}