#ifndef WEBCC_CONFIG_H_
#define WEBCC_CONFIG_H_

// Compile configurations.

// Set 1/0 to enable/disable logging.
#define WEBCC_ENABLE_LOG 1

#if WEBCC_ENABLE_LOG
// 0:VERB, 1:INFO, 2:WARN, 3:ERRO or 4:FATA
#define WEBCC_LOG_LEVEL 2
#endif

// Set 1/0 to enable/disable SSL/HTTPS.
#define WEBCC_ENABLE_SSL 1

// Set 1/0 to enable/disable GZIP compression.
#define WEBCC_ENABLE_GZIP 0

#endif  // WEBCC_CONFIG_H_
