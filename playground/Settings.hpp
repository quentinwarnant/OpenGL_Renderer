#pragma  once

//TMP: make sure to replicate changes in the Shader fragment program (in the future we could simply pass these as uniforms)
const int MAX_POINT_LIGHTS = 4; 
const int MAX_SPOT_LIGHTS = 2;
const int MAX_POINT_AND_SPOT_LIGHTS = MAX_POINT_LIGHTS+MAX_SPOT_LIGHTS;
