/*!
 * @file
 * @brief This file contains implementation of cpu side for phong shading.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 *
 */

#include <assert.h>
#include <math.h>

#include <student/buffer.h>
#include <student/bunny.h>
#include <student/camera.h>
#include <student/gpu.h>
#include <student/linearAlgebra.h>
#include <student/mouseCamera.h>
#include <student/student_cpu.h>
#include <student/student_pipeline.h>
#include <student/student_shader.h>
#include <student/swapBuffers.h>
#include <student/uniforms.h>
#include <student/vertexPuller.h>

#include <student/globals.h>

/**
 * @brief This structure contains all global variables for this method.
 */
struct PhongVariables {
  /// This variable contains GPU handle.
  GPU gpu;
  /// This variable contains light poistion in world-space.
  Vec3 lightPosition;

  ProgramID program;  // program id

  VertexPullerID puller;  // puller id

  BufferID vertices;  // buffer id

  BufferID indx;  // buffer id

} phong;  ///<instance of all global variables for triangle example.

/// \addtogroup cpu_side Úkoly v cpu části
/// @{

void phong_onInit(int32_t width, int32_t height) {
  // create gpu
  phong.gpu = cpu_createGPU();
  // set viewport size
  cpu_setViewportSize(phong.gpu, (size_t)width, (size_t)height);
  // init matrices
  cpu_initMatrices(width, height);
  // init lightPosition
  init_Vec3(&phong.lightPosition, 1000.f, 1000.f, 1000.f);

/// \todo Doprogramujte inicializační funkci.
/// Zde byste měli vytvořit buffery na GPU, nahrát data do bufferů, vytvořit
/// vertex puller a správně jej nakonfigurovat, vytvořit program, připojit k
/// němu shadery a nastavit interpolace.
/// Také byste zde měli zarezervovat unifromní proměnné pro matice, pozici
/// kamery, světla a další vaše proměnné.
/// Do bufferů nahrajte vrcholy králička (pozice, normály) a indexy na vrcholy
/// ze souboru bunny.h.
/// Shader program by měl odkazovat na funkce/shadery v souboru
/// student_shader.h.
/// V konfiguraci vertex pulleru nastavte dvě čtecí hlavy.
/// Jednu pro pozice vrcholů a druhou pro normály vrcholů.
/// Nultý vertex/fragment atribut by měl obsahovat pozici vertexu.
/// První vertex/fragment atribut by měl obsahovat normálu vertexu.
/// Budete využívat minimálně 4 uniformní proměnné
/// Uniformní proměnná pro view matici by měla být pojmenována "viewMatrix".
/// Uniformní proměnná pro projekční matici by měla být pojmenována
/// "projectionMatrix".
/// Uniformní proměnná pro pozici kamery by se měla jmenovat "cameraPosition".
/// Uniformní proměnná pro pozici světla by se měla jmenovat "lightPosition".
/// Je důležité udržet pozice atributů a názvy uniformních proměnných z důvodu
/// akceptačních testů.
/// Interpolace vertex atributů do fragment atributů je také potřeba nastavit.
/// Oba vertex atributy nastavte na \link SMOOTH\endlink interpolaci -
/// perspektivně korektní interpolace.<br>
/// <b>Seznam funkcí, které jistě využijete:</b>
///  - cpu_reserveUniform()
///  - cpu_createProgram()
///  - cpu_attachVertexShader()
///  - cpu_attachFragmentShader()
///  - cpu_setAttributeInterpolation()
///  - cpu_createBuffers()
///  - cpu_bufferData()
///  - cpu_createVertexPullers()
///  - cpu_setVertexPullerHead()
///  - cpu_enableVertexPullerHead()
///  - cpu_setIndexing()

  cpu_reserveUniform(phong.gpu,  // gpu
                     "projectionMatrix",   // uniform name
                     UNIFORM_MAT4);        // uniform type

  cpu_reserveUniform(phong.gpu,  // gpu
                     "viewMatrix",         // uniform name
                     UNIFORM_MAT4);        // uniform type

  cpu_reserveUniform(phong.gpu,  // gpu
                     "lightPosition",         // uniform name
                     UNIFORM_VEC3);        // uniform type

  cpu_reserveUniform(phong.gpu,  // gpu
                     "cameraPosition",         // uniform name
                     UNIFORM_VEC3);        // uniform type

  phong.program = cpu_createProgram(phong.gpu);

  cpu_attachVertexShader(phong.gpu,            // gpu
                         phong.program,        // program id
                         phong_vertexShader);  // pointer to function

  cpu_attachFragmentShader(phong.gpu,              // gpu
                           phong.program,          // program id
                           phong_fragmentShader);  // pointer to

  //! [INTERPOLATION]
  cpu_setAttributeInterpolation(
      phong.gpu,      // gpu
      phong.program,  // program id
      0,                        // vertex attribute index
      ATTRIB_VEC3, SMOOTH);  // interpolation type - with perspective correction

  cpu_setAttributeInterpolation(
      phong.gpu,      // gpu
      phong.program,  // program id
      1,                        // vertex attribute index
      ATTRIB_VEC3, SMOOTH);  // interpolation type - with perspective correction
  //! [INTERPOLATION]

  //! [BUFFER]
  cpu_createBuffers(
      phong.gpu,         // gpu
      2,                           // number of buffer ids that will be reserved
      &phong.vertices);  // pointer to buffer id variable

  cpu_createBuffers(
      phong.gpu,         // gpu
      1,                           // number of buffer ids that will be reserved
      &phong.indx);  // pointer to buffer id variable


  cpu_bufferData(
      phong.gpu,       // gpu
      phong.vertices,  // buffer id
      sizeof(bunnyVertices),  // size of data that is going to be copied to buffer
      bunnyVertices);         // pointer to data
  //! [BUFFER]

  cpu_bufferData(
      phong.gpu,       // gpu
      phong.indx,  // buffer id
      sizeof(bunnyIndices),  // size of data that is going to be copied to buffer
      bunnyIndices);         // pointer to data
  //! [BUFFER]

  //! [PULLER]
  cpu_createVertexPullers(
      phong.gpu,       // gpu
      1,                         // number of puller ids that will be reserved
      &phong.puller);  // pointer to puller id variable
  //! [PULLER]

  //! [HEAD]
  cpu_setVertexPullerHead(phong.gpu,       // gpu
                          phong.puller,    // puller id
                          0,                         // id of head/vertex attrib
                          phong.vertices,  // buffer id
                          sizeof(float) * 0,         // offset
                          sizeof(float) * 6);        // stride


  cpu_setVertexPullerHead(phong.gpu,       // gpu
                          phong.puller,    // puller id
                          1,                         // id of head/vertex attrib
                          phong.vertices,  // buffer id
                          sizeof(float) * 3,         // offset
                          sizeof(float) * 6);        // stride

  cpu_enableVertexPullerHead(phong.gpu,     // gpu
                             phong.puller,  // puller id
                             0);  // id of head/vertex attrib

  cpu_enableVertexPullerHead(phong.gpu,     // gpu
                             phong.puller,  // puller id
                             1);  // id of head/vertex attrib
  //! [HEAD]



  cpu_setIndexing(
    phong.gpu,  // gpu
    phong.puller, // puller id
    phong.indx, // id of buffer
    sizeof(float)); //size in bytes of one index

}

/// @}

void phong_onExit() { cpu_destroyGPU(phong.gpu); }

/// \addtogroup cpu_side
/// @{

void phong_onDraw(SDL_Surface* surface) {
  assert(surface != NULL);

  // clear depth buffer
  cpu_clearDepth(phong.gpu, +INFINITY);
  Vec4 color;
  init_Vec4(&color, .1f, .1f, .1f, 1.f);
  // clear color buffer
  cpu_clearColor(phong.gpu, &color);

/// \todo Doprogramujte kreslící funkci.
/// Zde byste měli aktivovat shader program, aktivovat vertex puller, nahrát
/// data do uniformních proměnných a
/// vykreslit trojúhelníky pomocí funkce cpu_drawTriangles.
/// Data pro uniformní proměnné naleznete v externích globálních proměnnénych
/// viewMatrix, projectionMatrix, cameraPosition a globální proměnné
/// phong.lightPosition.<br>
/// <b>Seznam funkcí, které jistě využijete:</b>
///  - cpu_useProgram()
///  - cpu_bindVertexPuller()
///  - cpu_uniformMatrix4fv()
///  - cpu_uniform3f()
///  - cpu_drawTriangles()
///  - getUniformLocation()

  //! [USE_PROGRAM]
  cpu_useProgram(phong.gpu,       // gpu
                 phong.program);  // program id
  //! [USE_PROGRAM]
  //! [BIND_PULLER]
  cpu_bindVertexPuller(phong.gpu,      // gpu
                       phong.puller);  // program id
  //! [BIND_PULLER]
  //! [SET_UNIFORMS]
  UniformLocation const viewMatrixUniform =
      getUniformLocation(phong.gpu,  // gpu
                         "viewMatrix");        // name of uniform variable

  cpu_uniformMatrix4fv(phong.gpu,    // gpu
                       viewMatrixUniform,      // location of uniform variable
                       (float *)&viewMatrix);  // pointer to data

  UniformLocation const projectionMatrixUniform =
      getUniformLocation(phong.gpu,  // gpu
                         "projectionMatrix");  // name of uniform variable

  cpu_uniformMatrix4fv(phong.gpu,      // gpu
                       projectionMatrixUniform,  // location of uniform variable
                       (float *)&projectionMatrix);  // pointer to data of
  //! [SET_UNIFORMS]


  UniformLocation const CamPosUniform = getUniformLocation(
    phong.gpu,
    "cameraPosition");

  cpu_uniform3f(
    phong.gpu,
    CamPosUniform,
    cameraPosition.data[0],
    cameraPosition.data[1],
    cameraPosition.data[2]);

  UniformLocation const lightPosUniform = getUniformLocation(
    phong.gpu,
    "lightPosition");

  cpu_uniform3f(
    phong.gpu,
    lightPosUniform,
    phong.lightPosition.data[0],
    phong.lightPosition.data[1],
    phong.lightPosition.data[2]);


  //! [DRAW]
  cpu_drawTriangles(phong.gpu,  // gpu
                    6276);                   // number of vertices
  //! [DRAW]


  // copy image from gpu to SDL surface
  cpu_swapBuffers(surface, phong.gpu);
}

/// @}