uniform float u_a;
uniform float u_s;
uniform bool u_isPhong;
uniform vec3 u_lightPos;
uniform vec3 u_obsPos;

varying vec3 N;

void main(void) {
    if(u_isPhong){
    ///PHONG
        // normalize vectors
        vec3 L = normalize(vec3(u_lightPos[0],-u_lightPos[2],u_lightPos[1]));
        vec3 R = normalize(-reflect(L,N));
        vec3 V = normalize(vec3(u_obsPos[0],-u_obsPos[2],u_obsPos[1]));
        // ambient
        float ambient   = u_a;
        // diffuse
        float diffuse   = dot(L, N);
        diffuse         = max(diffuse, 0.0);
        //specular
        float specular      = dot(R,V);
        specular            = max(specular, 0.0);
        specular            = pow(specular,0.3*u_s);
        // variable for vertex shader
        gl_FragColor   = (ambient+diffuse+specular) * gl_Color;
    }else{
    ///GOURAUD
        gl_FragColor = gl_Color;
    }
}
