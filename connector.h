#ifndef CONNECTOR_H
#define CONNECTOR_H
template<typename FType> class Connector;
template<typename Arg, typename Ret> class Connector<Ret(Arg)>{
    Connector(){
        Arg a;
    }
};
template<typename T> void connector(T funkcja){
    new Connector<T>;
}

#endif // CONNECTOR_H
