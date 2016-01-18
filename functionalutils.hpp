#ifndef FUNCTIONALUTILS_HPP
#define FUNCTIONALUTILS_HPP

#include <functional>
#include <QList>
#include <QDebug>

template< template<typename T> class ContainerType,
         class ReturnValueType,
         class ArgumentType>
ContainerType<ReturnValueType> mapContainer(ContainerType<ArgumentType> input,
                                            std::function<ReturnValueType(ArgumentType)> f)
{
    ContainerType<ReturnValueType> mapped;
    for(int i=0;i<input.size();i++){
        mapped.append(f(input[i]));
    }
    return mapped;
}
template<template<typename ReturnValueType> class ContainerType,
         class FunctionType>
ContainerType<typename FunctionType::result_type> mapC(
        ContainerType<typename FunctionType::argument_type> input,
        FunctionType f)
{
    return mapContainer<ContainerType,typename FunctionType::result_type,typename FunctionType::argument_type>(input,f);
}
#endif // FUNCTIONALUTILS_HPP
