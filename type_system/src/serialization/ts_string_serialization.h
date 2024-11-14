//
// Created by Lukas Brand on 02.07.2023.
//

#ifndef CSPOT_APPS_TS_STRING_SERIALIZATION_H
#define CSPOT_APPS_TS_STRING_SERIALIZATION_H

#include "type_system/ts_types.h"

bool serialize_value_string(struct ts_value *value);
bool deserialize_value_string(struct ts_value *value);

#endif // CSPOT_APPS_TS_STRING_SERIALIZATION_H
