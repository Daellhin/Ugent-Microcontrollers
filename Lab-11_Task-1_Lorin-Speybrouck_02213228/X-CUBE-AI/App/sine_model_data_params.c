/**
  ******************************************************************************
  * @file    sine_model_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Tue Apr 30 17:06:10 2024
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "sine_model_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_sine_model_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_sine_model_weights_array_u64[433] = {
  0xbee8c9ea3ee278fdU, 0x3e251270be0979e2U, 0x3eede60d3ed078f8U, 0x3e6c52edbcd31940U,
  0x3f0b40c33f0c0da2U, 0x3ec7a05fbee4fba6U, 0x3e169b013d890d68U, 0x3decae6bbee49efcU,
  0x3d492909U, 0xbe512fd100000000U, 0xbe6dab08bd9232c1U, 0xbdb6e49e00000000U,
  0x387d13983e817be2U, 0xbd8d5a6300000000U, 0xbe85c94fbe485288U, 0xbe8de98f00000000U,
  0x3ecc31d53e881fb1U, 0x3e0ebfdebe88f1feU, 0xbedccf143e59b6d4U, 0x3ecab1dfbe0cf12aU,
  0xbd031a36be8b8341U, 0x3d547c963bf04f00U, 0x3de53a7cbe8f4391U, 0xbc5bea20bd93574cU,
  0x3e0429263dfbab71U, 0xbee4269bbdf5903cU, 0xbe49e86ebe64aa4aU, 0x3e325b253e824113U,
  0xbeb901483ec1667dU, 0x3deb3592bea19518U, 0xbe8934b3bf2ebffbU, 0x3ca2a35cbe05418eU,
  0xbe338c543eae37d9U, 0xbdb59ce63e8eabe7U, 0x3ee47a9dbead6310U, 0xbe1831cf3e8452c5U,
  0x3de0a9c23e663636U, 0xbeb6c3eabd9ee830U, 0x3e458f6cbdef9e27U, 0x3ef0071fbcb4d2d0U,
  0xbe9aa9fabe9353afU, 0x3ec53353be1e5b98U, 0x3eadb2f63de482dcU, 0x3ebdd0803e5ff49eU,
  0x3cc9928c3e8c73e5U, 0x3e8adcbe3eb934ffU, 0xbe081c713ec7a2c1U, 0x3ecdeb853e9bf6f3U,
  0x3eb980b13ec5a227U, 0x3d7fe2bebe427416U, 0xbcffbc703ebd3b52U, 0xbe84f91d3e30baa2U,
  0x3eae95a4bed24fc7U, 0x3ebda231bd9cf184U, 0xbcd26d2b3eca8530U, 0xbea425ea3e1ae41aU,
  0xbe30aedabe20d380U, 0xbe2cdc1c3ec6909fU, 0x3ed9e3d13d3d1180U, 0xbeb425aabe1a1906U,
  0x3da4fff4be1d9e7aU, 0xbe9a0992be6eb9b5U, 0xbe4ad7dbbc5901c0U, 0xbe32ddf43c0c4d20U,
  0xbd8f24a03e55215cU, 0xbde93af2beb4e141U, 0xbe8cec05beccef14U, 0xbea99f80be902b5dU,
  0x3dfe31933e60658cU, 0x3e9466c3be9e8847U, 0xbe1066a33e59d053U, 0x3e5c3c953eabdecfU,
  0xbedc7d8f3e8d7ef1U, 0x3e978763bebb2920U, 0xbf33a2dfbe3c9325U, 0x3c0ad4a4bbe4bf00U,
  0xbde8bc34be5c51eaU, 0xbe9a5bfabe7dafbbU, 0x3e4239d23d31aeb0U, 0xbeaf7da6bd0a8cf0U,
  0xbec85f78bd43378eU, 0x3ea78bf3bd7f0e68U, 0x3ed1e5873e5ba8ebU, 0x3e124b1cbde0c150U,
  0xbe0c0b4dbed14a24U, 0x3ece40fc3a80c800U, 0x3e94382f3edc1b06U, 0x3eff13fcbeccd97aU,
  0xbe8c9aa03e8a4b31U, 0x3e8b985cbc089020U, 0xbdb7702a3e8c2d53U, 0xbe1aeda13da7cc54U,
  0xbef3e6ad3e0d628dU, 0x3e05cdbebd8d14f0U, 0x3e25bd65beba4ac8U, 0xbc4de160be5de612U,
  0x3eab0ebfbec3ca5bU, 0xbc8259b0bebab6b0U, 0xbbd5c8803eaf7225U, 0xbea4b0ebbed5dc68U,
  0xbea43f71be68fba8U, 0xbe3bcf06bda5a9e8U, 0x3d6d12a0bea0ffe8U, 0x3db5580cbe9e52d7U,
  0xbea394093b859011U, 0xbea2112f3ec12237U, 0x3d92f0603e98e698U, 0xbe064fd93e12200eU,
  0x3e3ef9383ede8070U, 0xbd7579e7bd96445cU, 0x3dbb6212bf32aab2U, 0x3d8e0f4dbe6b0defU,
  0xbed056453e8521f6U, 0x3ead5aa8be12c390U, 0x3e7b21173ee899b4U, 0x3b3ef8543d99e2e4U,
  0xbeaf867bbebf8071U, 0x3dfd21323e38bc06U, 0x3f035e2c3eeb137aU, 0x3e32ab42be6b7c2aU,
  0x3e187b723e5cd556U, 0x3e971b7fbeb4adbaU, 0x3ebf83283ebcb2c7U, 0x3e164f9b3eaf7d0bU,
  0x3ec50200bdb74cc8U, 0x3db8df6cbe8633cfU, 0x3eb26022be093d48U, 0xbe8432fb3e058f92U,
  0x3e4bbc02be7941f9U, 0x3e0f0e533e93796dU, 0x3ddee134bed3c931U, 0xbde80b4a3dcf29fcU,
  0x3ec619093d7c569eU, 0x3e777dd6be6cb53bU, 0x3d01cde93ee8af91U, 0x3ecf14a23e003f2eU,
  0xbe197d083e24013eU, 0x3e4ed660be4cb811U, 0x3ce423083e605f10U, 0x3ed957b73e09101aU,
  0xbd944934beaa7c06U, 0x3e361429be7e5469U, 0x3ef508213d84dfd2U, 0x3f03e90dbe89a9eeU,
  0x3e421768bb4f4fb6U, 0xbdb5489abe154995U, 0x3dab7c44U, 0x3e4d1ff73d7ccfa0U,
  0x3df29f8fbe959b6dU, 0x3e856ca900000000U, 0x3c44df5fbea922d4U, 0xbdf47f3abe7e204aU,
  0xbed4634a3dc6195fU, 0xbded4d2bbe6d8409U, 0xbe1e1cb4bea78282U, 0x3e6c5e453eb89871U,
  0x3eb9556abde4b190U, 0x3ea9a5383ed0afe3U, 0x3db00f48be1bfc60U, 0x3e813604bea495efU,
  0xbea1aa1fbedc2783U, 0xbdca8004becb1072U, 0xbec4f7b83ccd0ae0U, 0xbb96fc80bd31b130U,
  0x3e305116be0884f2U, 0xbeb054a7be64634cU, 0x3daf2e243eb72909U, 0xbea2ccfcbd984c30U,
  0xbf03476dbe55b658U, 0x3dd89e923ea6cf3aU, 0x3ecb28a73e3aaa44U, 0xbe91035b3c75a36bU,
  0xbd7010723eb99771U, 0xbe3d8931bec58d9cU, 0xbe863cad3f1df9eeU, 0x3e422b2e3db48500U,
  0x3ea629e0be2ab6a9U, 0x3e3431633eca9740U, 0x3ebed98b3e889e0bU, 0xbf037c56be3017eeU,
  0x3e22acb9beb27153U, 0x3ebf2ce0bdc00170U, 0x3c2648133dbcc545U, 0xbe88b4c0be8536f4U,
  0xbf178212bea60b9cU, 0x3e3cf2aebd2d8928U, 0x3dd03e143e2743d8U, 0xbe9c1fa0bdca4910U,
  0xbeac850f3e82675eU, 0xbe9859b8bd8e5b8cU, 0x3ec7611f3e37e69cU, 0x3e4e85ac3ec30be3U,
  0x3ea40f173e857ae9U, 0xbdcd694b3e9d33f4U, 0x3ebf4ac9be904db6U, 0x3da5ed3cbed5bda8U,
  0xbe368c76beda2252U, 0xbe6a5bab3e293066U, 0x3e9fc9ca3df72e27U, 0xbe1d387b3ea0c2c2U,
  0x3eb9d2ab3dcd19baU, 0xbdafbc0abca48810U, 0x3b83e5803ec94c61U, 0xbee5aeb73e4ccea8U,
  0x3e2c7874bf0ef6abU, 0x3e9597683c2a2380U, 0xbe90f892bed34b45U, 0xbea8c4b5be811c51U,
  0xbe8c6ababd6760e0U, 0xbea5fe11be8b34e8U, 0xbdbc2e80bcb9d8a0U, 0xbec4acff3eb9dcd5U,
  0xbe4d52463dbbd07cU, 0xbe27a1ce3e5ba6c2U, 0x3d80716cbd0beca8U, 0xbecb37c1bec5b542U,
  0xbe8fb5c6be9800f8U, 0x3c8ee8593b85b802U, 0xbe98a52abeb07ef1U, 0x3ecef5b1be292798U,
  0x3dafa88ebccdf752U, 0xbe90f966be139a42U, 0x3e33690cbdf21ee5U, 0x3cff0f513eca082bU,
  0x3dd4896d3d14b92aU, 0xbec399753ec7734eU, 0xbedda7b03e651aa5U, 0xbe926715bedc82acU,
  0x3e7d7aa7be9afde6U, 0xbe9ce1b8becd5e4aU, 0xbda723923eb08f8fU, 0x3d8635fc3e782ec2U,
  0x3ed5cf51bd83b5e0U, 0xbe5241673ea8df33U, 0x3ecc7517bed28470U, 0xbe9661053cb1ece3U,
  0x3e4f87d5bd9a9ca2U, 0x3d1953af3e45f51aU, 0x3e988f573e645568U, 0x3ed4a4fa3e9b03dfU,
  0x3dab8e9abdf972d5U, 0xbda42071be4f1ca2U, 0xbdc409e83eafde80U, 0xbe8abd56be1800a6U,
  0xbe1d954c3d9805bcU, 0x3e8fd0ed3e780096U, 0x3d11f496bec77cccU, 0x3d81b772bc6cf2f3U,
  0x3e3569653d0ab3c4U, 0xbe411113bec2b062U, 0xbe84f7d5bb8e89e3U, 0x3ee55ed2be5ff43fU,
  0x3dc5daacbea6c69eU, 0x3e85cdae3ea8e3e3U, 0xbed9722a3d56ea60U, 0x3daaae9c3e8af63aU,
  0xbedc190e3dde4af6U, 0x3e10fee13e459adeU, 0xbe0674803da105dbU, 0xbe0de5333e876097U,
  0xbe13aa2d3c195e63U, 0x3ebb87df3e04698eU, 0x3e8c0759be7613d9U, 0xbde706b8bea99b84U,
  0x3f05c5c0becde2c7U, 0xbe0450663e4d9d56U, 0xbd412da0bd028420U, 0x3db6d9b4bdf0ce45U,
  0xbdc1d4e63e4874abU, 0x3e11fc23be2628b4U, 0x3e14cbf33d718bf7U, 0xbd710e483e4e986fU,
  0xbf1468e9bdc9b4a8U, 0x3edf91f83eecf208U, 0xbe8654383eb87b34U, 0x3e401bc8be971ae2U,
  0xbe89bc12be08630aU, 0x3df9aac7be6d9e7dU, 0xbe83ebbcbdb534e1U, 0xbe98abb73e3d9115U,
  0xbc270a59U, 0x3e04e477beadbaa0U, 0xbbb5fbe1bc239f05U, 0x3e707cf0U,
  0xbd1f8fd7bb1a6953U, 0xbc8efd11bd438651U, 0x3c3461bf3e0d4eafU, 0x3d22198fbe125b6dU,
  0xbe3effafbef4d6a9U, 0x3ebb89603f8a0f5aU, 0x3da4972cbe902113U, 0x3d675be83f30823bU,
  0x3eaf911abe95f322U, 0x3d81714d3e3e5da0U, 0xbe92e9f4be102ffaU, 0x3ed7f4653eabd8ffU,
  0x3df1d40cbe9c491eU, 0x3eb77eefbc31a140U, 0x3eb21d5fbe92bf58U, 0xbeb28935bea7ef13U,
  0xbed2ab443e9c00d1U, 0x3d94ab32bdc07544U, 0xbeaf5d333e0c40c7U, 0xbe83dbaebec983e9U,
  0x3e42c60abe732f36U, 0xbe6f62e8bd233cc0U, 0xbe22c158bea635dcU, 0xbabda900be0e713eU,
  0x3dbdd1743e5d513eU, 0xbd53cdd8bd8a225cU, 0xbdd9487c3e94e2a5U, 0x3d4f3ee0bd0188e0U,
  0xbedb4833baa3432dU, 0x3e50a9edbf1c5233U, 0xbe5e0bd93e89a49aU, 0xbe165508bed05f18U,
  0x3e5b83cabe9853efU, 0xbe7ff541be9a28dbU, 0x3d832efd3ecf4591U, 0x3ebde4f9be099756U,
  0xbe52d9863d93a311U, 0xbed1f9663f4fe8b6U, 0xbcb9e504be50b0eaU, 0x3e4b93623f100ab7U,
  0x3eafa0a63ec92433U, 0x3e6d17193df4293cU, 0x3edfa1d5bf201c45U, 0xbe0f80303ee21c71U,
  0xbe865ad2bf059231U, 0xbde021e43d856a24U, 0x3e0b36e6be119be6U, 0xbe1281883ed1f8d4U,
  0x3eacd377bda7f774U, 0xbef83f70be2d9c6dU, 0xbf0ec1c4bcef47abU, 0x3d30f460beb262fdU,
  0x3e52dd26bec51c83U, 0xbd82a3f43eb0f203U, 0x3ebdc45f3eb26d0dU, 0xbebda05a3c9b8ff0U,
  0xbe80805abedac641U, 0x3d0b0500bed339d4U, 0xbd9346a0bdc9ea14U, 0xbe9c3db8beb2ca83U,
  0x3e85dbbbbe13712fU, 0x3ea62dd5bf78119cU, 0xbdb93e003e9b96fcU, 0x3dca8394be9ebc84U,
  0x3e515686be8ed30bU, 0xbe23c1f0be1ec5d0U, 0x3d03dd08be596463U, 0x3dc84edcbe6dd110U,
  0xbea3cc8bbe0d468dU, 0x3e473fe2bf1e2d38U, 0x3e300d803ec23c89U, 0xbe170a5cbef030beU,
  0xbebd1fe13e597ee5U, 0xbe648ac73e6a916fU, 0x3daab843beb9be27U, 0x3ebbd51b3e799492U,
  0x3ed147453ed32ac1U, 0xbdf9889abf59dbd3U, 0x3e48235d3e5c0183U, 0x3dd45194bd9590cfU,
  0xbe8cac803e4a0c49U, 0x3dd80a6c3dd6a1d1U, 0xbe015c8dbd6d06e5U, 0x3ef1d853bddfec98U,
  0xbc9f7b103d4cc32aU, 0xbede88193d94015cU, 0x3ea1d369bded8c68U, 0xbd82eb683ebad47dU,
  0xbeb6d65abece1d58U, 0x3e48d2e8bcf117a8U, 0xbe9cade43c7b4588U, 0xbdeccde4becadda3U,
  0xbe15d064be774034U, 0x3e856f413ed60689U, 0x3e89791f3e0e01fcU, 0x3e0fa7723ed0b805U,
  0x3e1addb3be8d5352U, 0x3e14fe3b3ebf253eU, 0x3e804fcdbe8960d8U, 0xbedbaa7e3dba124bU,
  0x3d94f9fc3e396e88U, 0xbdfecea9be89d50eU, 0xbd6425b8be94c730U, 0x3d525ec03d50a3bdU,
  0xbebf6884bd382408U, 0xbe16f3eabeb9428dU, 0xbe2813c03e9021e2U, 0x3e3fd3cebe4cb2a1U,
  0xbe4ff3503ed0d62eU, 0xbe43296abf71e2abU, 0xbde3941c3e23d5b3U, 0xbe602a2cbe27a3a8U,
  0x3d5346923e8d6bd1U, 0x3e5ccce53e8d2663U, 0x3e07b67ebcf5e8f5U, 0xbe618e773ec2be76U,
  0xbecee7743d92bb00U, 0x3ea9eb02be7d78fbU, 0x3da2cd0c3eb35c13U, 0x3ed087bd3ccb49b2U,
  0x3dc924fe3c7e1880U, 0xbec2af61bd4d368fU, 0xbf0328623ed7ad73U, 0xbf0fac6dbdee7340U,
  0xbe1db5523ee4c715U, 0x3eefbc2abf2a9943U, 0xbc91f3d03e0776f5U, 0xbe9825a0bf3bef58U,
  0x3e67bf963dc3226eU, 0x3d2ffd8dbe862dafU, 0x3cf0ae09bf062530U, 0xbe819282be55f596U,
  0xbc9535623d4405adU, 0x3da281b900000000U, 0x3d23b3bb3d2d0f95U, 0xbcaa58ba00000000U,
  0x3d343b2cbd131c2bU, 0x3d37360cbd211da6U, 0xbd5c41783d3059b2U, 0x3da638613e03563cU,
  0x3f0570483f2230adU, 0xbf1a5254bec0c8c2U, 0xbf5c62593e336eccU, 0xbed1174bbef4e0a7U,
  0xbf02e771be932ec2U, 0x3f1aa0f13d964cefU, 0xbf01db83be882317U, 0xbe93c756bf26cd90U,
  0x3d403742U,
};


ai_handle g_sine_model_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_sine_model_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

