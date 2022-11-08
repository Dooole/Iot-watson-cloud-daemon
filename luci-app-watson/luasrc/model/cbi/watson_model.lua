
map = Map("watson")

section = map:section(NamedSection, "watson_sct", "watson", "Settings")

flag = section:option(Flag, "enable", "Enable", "Enable program")

orgid = section:option( Value, "orgid", "Organization ID")

typeid = section:option( Value, "typeid", "Type ID")

deviceid = section:option( Value, "deviceid", "Device ID")

token = section:option( Value, "token", "Token")
token.datatype = "string"
token.maxlength = 32

return map