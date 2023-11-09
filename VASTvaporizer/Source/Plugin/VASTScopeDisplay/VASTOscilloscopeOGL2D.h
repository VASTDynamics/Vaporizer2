/*
VAST Dynamics
*/
#ifndef VASTOSCILLOSCOPEOGL2D_H_INCLUDED
#define VASTOSCILLOSCOPEOGL2D_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../../Engine/VASTSingleNote.h"
#include "VASTRingBuffer.h"

/** This 2D Oscilloscope uses a Fragment-Shader based implementation.
 
    Future Update: modify the fragment-shader to do some visual compression so
    you can see both soft and loud movements easier. Currently, the most loud
    parts of a song to a bit too far out of the frame and the soft parts don't
    always move the wave very much.
 */

#define OGL2D_RING_BUFFER_SIZE 11025 * 10 //0.25s * 10
#define OGL2D_VISUAL_SIZE 512

class VASTAudioProcessorEditor; //forward declaration
class VASTOscilloscopeOGL2D :  public Component,
                        public OpenGLRenderer
{
    
public:
    
	VASTOscilloscopeOGL2D(VASTAudioProcessorEditor *editor, std::unique_ptr<VASTRingBuffer<GLfloat>> * ringBuffer);
	~VASTOscilloscopeOGL2D();
    
    //==========================================================================
    // VASTOscilloscopeOGL2D Control Functions
    
	void start();
	void stop();    
	
	void lookAndFeelChanged() override;

    //==========================================================================
    // OpenGL Callbacks
    
    /** Called before rendering OpenGL, after an OpenGLContext has been associated
        with this OpenGLRenderer (this component is a OpenGLRenderer).
        Sets up GL objects that are needed for rendering.
     */
	void newOpenGLContextCreated() override;
    
    /** Called when done rendering OpenGL, as an OpenGLContext object is closing.
        Frees any GL objects created during rendering.
     */
	void openGLContextClosing() override;
    
    
    /** The OpenGL rendering callback.
     */
	void renderOpenGL() override;
    //==========================================================================
    // JUCE Callbacks
    
	void paint(Graphics& g) override;
    
	void resized() override;
    
private:
    
    //==========================================================================
    // OpenGL Functions
    
	VASTAudioProcessorEditor* myEditor = nullptr;
	juce::Colour m_osciColour;

    /** Loads the OpenGL Shaders and sets up the whole ShaderProgram
    */
	void createShaders();
    
    //==============================================================================
    // This class just manages the uniform values that the fragment shader uses.
    struct Uniforms
    {
        Uniforms (OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
        {
            //projectionMatrix = createUniform (openGLContext, shaderProgram, "projectionMatrix");
            //viewMatrix       = createUniform (openGLContext, shaderProgram, "viewMatrix");
            
			//resolutionXY = createUniform (openGLContext, shaderProgram, "resolutionXY");
            //audioSampleData     = createUniform (openGLContext, shaderProgram, "audioSampleData");
            
            osciColour = createUniform(openGLContext, shaderProgram, "osciColour");
        }
        
        //std::unique_ptr<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix;
        //std::unique_ptr<OpenGLShaderProgram::Uniform> resolutionXY, osciColour;
        std::unique_ptr<OpenGLShaderProgram::Uniform> osciColour;
        
    private:
        static std::unique_ptr<OpenGLShaderProgram::Uniform> createUniform (OpenGLContext& openGLContext,
                                                            OpenGLShaderProgram& shaderProgram,
                                                            const char* uniformName)
        {
            if (openGLContext.extensions.glGetUniformLocation (shaderProgram.getProgramID(), uniformName) < 0)
                return nullptr;
            
            return std::make_unique<OpenGLShaderProgram::Uniform>(shaderProgram, uniformName);
        }
    };
        
    // OpenGL Variables
    OpenGLContext openGLContext;
    GLuint VBO, VAO, EBO;
    
    std::unique_ptr<OpenGLShaderProgram> shader;
    std::unique_ptr<Uniforms> uniforms;
    
    std::unique_ptr<OpenGLShaderProgram::Attribute> position_;

    const char* vertexShader;
    const char* fragmentShader;
    
	GLfloat oscivertices[OGL2D_VISUAL_SIZE * 2];
	GLuint osciindices[OGL2D_VISUAL_SIZE * 2]; 

    // Audio Buffer
	std::unique_ptr<VASTRingBuffer<GLfloat>> * ringBuffer;
    AudioBuffer<GLfloat> readBuffer;    // Stores data read from ring buffer
    //GLfloat visualizationBuffer [OGL2D_VISUAL_SIZE];    // Single channel to visualize       
    
	bool m_bContRepaint = false;
    // Overlay GUI
    //Label statusLabel;
            
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VASTOscilloscopeOGL2D)
};
#endif
