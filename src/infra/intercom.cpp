#include "infra/intercom.h"
#include "infra/fs.h"
#include "utils/print.h"

extern StateMachineController& stateMachineController;

void configureIntercom(String kmnModel, int firstApartment) {
  KMNModel model;

  if (kmnModel == "Vizit") {
    model = KMNModel::VIZIT;
  } else if (kmnModel == "Cyfral") {
    model = KMNModel::CYFRAL;
  } else {
    println("Wrong intercom model: ", kmnModel);
    return;
  }

  stateMachineController.configure(model, firstApartment);
  println("Intercom configured: ", kmnModel, ", first appartment: ", firstApartment);
}


bool loadIntercomConfig() {
  DynamicJsonDocument doc(1024);

  bool success = readJsonVariantFromFile(INTERCOM_SETTINGS_PATH, doc);

  if (!success)
    return false;

  JsonVariant root = doc.as<JsonVariant>();

  configureIntercom(root["kmnModel"], root["firstApartment"]);

  return true;
}

void initIntercom() {
  if (!loadIntercomConfig() && FACTORY_STATIC_LOCAL_IP) {
    configureIntercom(
      FACTORY_KMN_MODEL,
      FACTORY_FIRST_APARTMENT
    );
  }
}

IntercomConnectionStatus getIntercomStatus() {
  return stateMachineController.getStatus();
}

int getLastCalledNumber() {
  return stateMachineController.getLastCalledNumber();
}

JsonArray getIntercomModels() {
  const size_t CAPACITY = JSON_ARRAY_SIZE(3);
  StaticJsonDocument<CAPACITY> modelsDoc;

  JsonArray models = modelsDoc.to<JsonArray>();
  models.add("Vizit");
  models.add("Cyfral");

  return models;
}

void getDefaultIntercomConf(JsonVariant& root) {
  root["kmnModel"] = FACTORY_KMN_MODEL;
  root["firstApartment"] = FACTORY_FIRST_APARTMENT;
  root["kmnModelList"] = getIntercomModels();
}