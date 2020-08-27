/*!
 * @file
 * @brief This file contains implemenation of phong vertex and fragment shader.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <assert.h>
#include <math.h>

#include <student/gpu.h>
#include <student/student_shader.h>
#include <student/uniforms.h>

/// \addtogroup shader_side Úkoly v shaderech
/// @{

void phong_vertexShader(GPUVertexShaderOutput *const      output,
                        GPUVertexShaderInput const *const input,
                        GPU const                         gpu) {
  /// \todo Naimplementujte vertex shader, který transformuje vstupní vrcholy do
  /// clip-space.<br>
  /// <b>Vstupy:</b><br>
  /// Vstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu ve
  /// world-space (vec3) a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).<br>
  /// <b>Výstupy:</b><br>
  /// Výstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu (vec3)
  /// ve world-space a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).
  /// Výstupní vrchol obsahuje pozici a normálu vrcholu proto, že chceme počítat
  /// osvětlení ve world-space ve fragment shaderu.<br>
  /// <b>Uniformy:</b><br>
  /// Vertex shader by měl pro transformaci využít uniformní proměnné obsahující
  /// view a projekční matici.
  /// View matici čtěte z uniformní proměnné "viewMatrix" a projekční matici
  /// čtěte z uniformní proměnné "projectionMatrix".
  /// Zachovejte jména uniformních proměnných a pozice vstupních a výstupních
  /// atributů.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Využijte vektorové a maticové funkce.
  /// Nepředávajte si data do shaderu pomocí globálních proměnných.
  /// Pro získání dat atributů použijte příslušné funkce vs_interpret*
  /// definované v souboru program.h.
  /// Pro získání dat uniformních proměnných použijte příslušné funkce
  /// shader_interpretUniform* definované v souboru program.h.
  /// Vrchol v clip-space by měl být zapsán do proměnné gl_Position ve výstupní
  /// struktuře.<br>
  /// <b>Seznam funkcí, které jistě použijete</b>:
  ///  - gpu_getUniformsHandle()
  ///  - getUniformLocation()
  ///  - shader_interpretUniformAsMat4()
  ///  - vs_interpretInputVertexAttributeAsVec3()
  ///  - vs_interpretOutputVertexAttributeAsVec3()
  (void)output;
  (void)input;
  (void)gpu;
  //! [INTERPRET_UNIFORMS]
  // get handle to all uniforms
  //! [INTERPRET_UNIFORMS]
  // get handle to all uniforms
  Uniforms const uniformsHandle = gpu_getUniformsHandle(gpu);  // gpu

  // get uniform location of view matrix
  UniformLocation const viewMatrixLocation =
      getUniformLocation(gpu,            // gpu handle
                         "viewMatrix");  // name of uniform variable

  // get pointer to view matrix
  Mat4 const *const view = shader_interpretUniformAsMat4(
      uniformsHandle,       // handle to all uniforms
      viewMatrixLocation);  // location of view matrix

  // get uniform location of projection matrix
  UniformLocation const projectionMatrixLocation =
      getUniformLocation(gpu,                  // gpu handle
                         "projectionMatrix");  // name of uniform variable

  // get pointer to projection matrix
  Mat4 const *const proj = shader_interpretUniformAsMat4(
      uniformsHandle,             // handle to all uniforms
      projectionMatrixLocation);  // location of projection matrix
  //! [INTERPRET_UNIFORMS]

  //! [INPUT_ATTRIBUTES]
  Vec3 const *const position =
      vs_interpretInputVertexAttributeAsVec3(gpu,    // gpu
                                             input,  // input vertex
                                             0);  // index of vertex attribute


  Vec3 const *const normal =
      vs_interpretInputVertexAttributeAsVec3(gpu,    // gpu
                                             input,  // input vertex
                                             1);  // index of vertex attribute
  //! [INPUT_ATTRIBUTES]

  //! [PROJECT]
  Mat4 mvp;
  multiply_Mat4_Mat4(&mvp, proj, view);

  Vec4 pos4;
  copy_Vec3Float_To_Vec4(&pos4, position, 1.f);

  multiply_Mat4_Vec4(
      &output->gl_Position,  // output - position of output vertex in clip-space
      &mvp,                  // projection view matrix
      &pos4);                // position of vertex in world-space
  //! [PROJECT]

  //! [VS_ATTRIBUTE]
  Vec3 *const colorAttribute =
      vs_interpretOutputVertexAttributeAsVec3(gpu,     // gpu
                                              output,  // output vertex
                                              0);  // index of vertex attribute

  //! [VS_ATTRIBUTE]
  Vec3 *const norm =
      vs_interpretOutputVertexAttributeAsVec3(gpu,     // gpu
                                              output,  // output vertex
                                              1);  // index of vertex attribute
  //! [VS_ATTRIBUTE]
  //! [VS_COLOR]
  init_Vec3(colorAttribute,                     // pointer to vertex attribute
            position->data[0],   //   x component 
            position->data[1],   //  y component 
            position->data[2]);  //  z component 


  init_Vec3(norm,                     // pointer to vertex attribute
            normal->data[0],   // x component 
            normal->data[1],   // y component 
            normal->data[2]);  // z component 

}


float clamp(float v, float lo, float hi) {
  if (v < hi) {
    if(v>lo){
      return v;
    }else{
      return lo;
    }
  }

  if(v>hi){
    return hi;
  }else{
    return v;
  }
}
void phong_fragmentShader(GPUFragmentShaderOutput *const      output,
                          GPUFragmentShaderInput const *const input,
                          GPU const                           gpu) {
  /// \todo Naimplementujte fragment shader, který počítá phongův osvětlovací
  /// model s phongovým stínováním.<br>
  /// <b>Vstup:</b><br>
  /// Vstupní fragment by měl v nultém fragment atributu obsahovat
  /// interpolovanou pozici ve world-space a v prvním
  /// fragment atributu obsahovat interpolovanou normálu ve world-space.<br>
  /// <b>Výstup:</b><br>
  /// Barvu zapište do proměnné color ve výstupní struktuře.<br>
  /// <b>Uniformy:</b><br>
  /// Pozici kamery přečtěte z uniformní proměnné "cameraPosition" a pozici
  /// světla přečtěte z uniformní proměnné "lightPosition".
  /// Zachovejte jména uniformních proměnný.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Dejte si pozor na velikost normálového vektoru, při lineární interpolaci v
  /// rasterizaci může dojít ke zkrácení.
  /// Zapište barvu do proměnné color ve výstupní struktuře.
  /// Shininess faktor nastavte na 40.f
  /// Difuzní barvu materiálu nastavte podle normály povrchu.
  /// V případě, že normála směřuje kolmo vzhůru je difuzní barva čistě bílá.
  /// V případě, že normála směřuje vodorovně nebo dolů je difuzní barva čiště zelená.
  /// Difuzní barvu spočtěte lineární interpolací zelené a bíle barvy pomocí interpolačního parameteru t.
  /// Interpolační parameter t spočtěte z y komponenty normály pomocí t = y*y (samozřejmě s ohledem na negativní čísla).
  /// Spekulární barvu materiálu nastavte na čistou bílou.
  /// Barvu světla nastavte na bílou.
  /// Nepoužívejte ambientní světlo.<br>
  /// <b>Seznam funkcí, které jistě využijete</b>:
  ///  - shader_interpretUniformAsVec3()
  ///  - fs_interpretInputAttributeAsVec3()
  (void)output;
  (void)input;
  (void)gpu;


  Uniforms const uniformsHandle = gpu_getUniformsHandle(
    gpu);

  UniformLocation const camPosLocation = getUniformLocation(
    gpu,
    "cameraPosition");

  UniformLocation const lightPosLocation = getUniformLocation(
    gpu,
    "lightPosition");


  Vec3 const*const cam = shader_interpretUniformAsVec3(
    uniformsHandle,
    camPosLocation);

  Vec3 const*const light = shader_interpretUniformAsVec3(
    uniformsHandle,
    lightPosLocation);

  Vec3 const*const pos_p = fs_interpretInputAttributeAsVec3 (
    gpu,
    input,
    0);


  Vec3 const*const norm_p = fs_interpretInputAttributeAsVec3 (
    gpu,
    input,
    1);

  Vec3 col;
  init_Vec3(&col, 0.f, 1.f, 0.f);

  Vec3 col_S;
  init_Vec3(&col_S, 1.f, 1.f, 1.f);

  float Shininess_factor = 40.f;

  Vec3 tmp;
  Vec3 tmp2;
  sub_Vec3(&tmp, light, pos_p);

  Vec3 L;
  normalize_Vec3(&L, &tmp);

  Vec3 N;
  normalize_Vec3(&N, norm_p);

  float dv=dot_Vec3(&N, &L);
  float dif;
  if (dv < 1.f) {
    if(dv>0.f){
      dif = dv;
    }else{
      dif = 0.f;
    }
  }else if(dv>1.f){
    dif = 1.f;
  }else{
    dif = dv;
  }
  sub_Vec3(&tmp, cam, pos_p);

  Vec3 V;
  normalize_Vec3(&V, &tmp);

  multiply_Vec3_Float(&tmp, &V, -1.f);

  Vec3 R;
  reflect(&R, &tmp, &N);
  normalize_Vec3(&R, &R);

  float speFa;
  dv = dot_Vec3(&R, &L);
  if (dv < 1.f) {
    if(dv>0.f){
      speFa = dv;
    }else{
      speFa = 0.f;
    }
  }else if(dv>1.f){
    speFa = 1.f;
  }else{
    speFa = dv;
  }
  speFa = (float)pow(speFa, Shininess_factor);

  float difLight = 1.f;
  float specularLight = 0.5f;

  Vec3 final;
  multiply_Vec3_Float(&tmp, &col, dif*difLight);
  multiply_Vec3_Float(&tmp2, &col_S, speFa*specularLight);

  add_Vec3(&final, &tmp, &tmp2);

  output->color.data[0] = final.data[0];
  output->color.data[1] = final.data[1];
  output->color.data[2] = final.data[2];
  output->color.data[3] = 1.f;
}


/// @}
