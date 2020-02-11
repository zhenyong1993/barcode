#include "barcode.h"
#include "json.h"
#include <iostream>
#include "mqClient.h"
using namespace std;

barcode::barcode()
{
    mqClient::setContext(this);
    mqClient::getInstance().connect();
}

void barcode::onGetStr(const string& str)
{
    string packet = generatePacket(str);
	cout << "get content is " << str << endl;
    cout << "json is " << packet << endl;
    mqClient::getInstance().publish("command/x/test", packet);
}

string barcode::generatePacket(const string& id)
{
    Json::Value val;
    val["encode"] = "false";
    val["id"] = "123";
    val["machineNo"] = "A131420035";
    val["type"] = "20";
    val["order"] = 253;
    val["dest"] = "A131420035";
        Json::Value content;
        content["cmdId"] = "toolCompensationfilwDownload";
        content["concurrent"] = "true";
            Json::Value data;
            data["toolIdentifier"] = id;
        content["data"] = data;
    val["content"] = content;
    return val.toStyledString();
    
}
void barcode::messageArrived(char* topicName, int topicLen, MQTTAsync_message* message)
{

}

void barcode::connectLost(char* cause)
{

}

void barcode::connectSuccess(MQTTAsync_successData* response)
{
    cout << "barcode connect success" << endl;
}

void barcode::connectFail(MQTTAsync_failureData* response)
{
    cout << "barcode connect fail" << endl;
}

void barcode::subscribeOk(MQTTAsync_successData* response)
{

}

void barcode::subscribeFail()
{

}
