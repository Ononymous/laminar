//
// Created by Lukas Brand on 02.07.2023.
//

#ifndef CSPOT_APPS_TS_JSON_CONVERTER_H
#define CSPOT_APPS_TS_JSON_CONVERTER_H

struct ts_value* value_from_json(const char* json);
char* value_to_json(struct ts_value* value);

#endif // CSPOT_APPS_TS_JSON_CONVERTER_H
