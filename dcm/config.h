#ifndef DCM_CONFIG_H_
#define DCM_CONFIG_H_

// Compile configurations.

// Set 1/0 to enable/disable logging.
#define DCM_ENABLE_LOG 1

#if DCM_ENABLE_LOG
// 0:VERB, 1:INFO, 2:USER, 3:WARN, 4:ERRO
#define DCM_LOG_LEVEL 2
#endif

#endif  // DCM_CONFIG_H_
