/*
VAST Dynamics
OpenGL Fragment-Shader based
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTOscilloscopeOGL2D.h"
#include "../VASTAudioProcessorEditor.h"
#include "VASTRingBuffer.h"

using namespace ::juce::gl;

VASTOscilloscopeOGL2D::VASTOscilloscopeOGL2D(VASTAudioProcessorEditor *editor, std::unique_ptr<VASTRingBuffer<GLfloat>> * ringBuffer)
: myEditor(editor), readBuffer (1, OGL2D_RING_BUFFER_SIZE) 
{
    openGLContext.setOpenGLVersionRequired (OpenGLContext::OpenGLVersion::defaultGLVersion);        
	openGLContext.setComponentPaintingEnabled(false);

    this->ringBuffer = ringBuffer;       
       
	m_bContRepaint = false;
	m_osciColour = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeLine);
}
    
VASTOscilloscopeOGL2D::~VASTOscilloscopeOGL2D()
{
    // Turn off OpenGL
    openGLContext.setContinuousRepainting (false);
    openGLContext.detach();
        
    // Detach ringBuffer
    ringBuffer = nullptr;
}
    
//==========================================================================
// VASTOscilloscopeOGL2D Control Functions
    
void VASTOscilloscopeOGL2D::start()
{
	openGLContext.setRenderer(this);
	openGLContext.attachTo(*this);
    openGLContext.setContinuousRepainting (true);
}
    
void VASTOscilloscopeOGL2D::stop()
{
    openGLContext.setContinuousRepainting (false);
}
    

void VASTOscilloscopeOGL2D::lookAndFeelChanged() {
	//if (uniforms != nullptr) {
		m_osciColour = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeLine);
		//uniforms->osciColour->set((GLfloat)(m_osciColour.getRed() / 256.0), (GLfloat)(m_osciColour.getGreen() / 256.0), (GLfloat)(m_osciColour.getBlue() / 256.0), (GLfloat)(m_osciColour.getAlpha() / 256.0));		
	//}
}
    
//==========================================================================
// OpenGL Callbacks
    
/** Called before rendering OpenGL, after an OpenGLContext has been associated
    with this OpenGLRenderer (this component is a OpenGLRenderer).
    Sets up GL objects that are needed for rendering.
    */
void VASTOscilloscopeOGL2D::newOpenGLContextCreated()
{
    // Setup Shaders
    createShaders();
	
    // Setup Buffer Objects
    openGLContext.extensions.glGenBuffers (1, &VBO); // Vertex Buffer Object
	openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);

    openGLContext.extensions.glGenBuffers (1, &EBO); // Element Buffer Object
	openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Element Buffer Object

	//int iii = openGLContext.getSwapInterval();
	//openGLContext.setSwapInterval(4);
	//openGLContext.setSwapInterval(8);
    openGLContext.setSwapInterval(1);
    
	for (int i = 0; i < OGL2D_VISUAL_SIZE; i++) {
		float t = i / (OGL2D_VISUAL_SIZE - 1.0f);
		oscivertices[i * 2] = 2.0f * t - 1.0f; //TODO can be stored once
		osciindices[i * 2] = i;
		osciindices[i * 2 + 1] = i + 1;
	}
	osciindices[2 * OGL2D_VISUAL_SIZE - 1] = OGL2D_VISUAL_SIZE - 1;
}
    
/** Called when done rendering OpenGL, as an OpenGLContext object is closing.
    Frees any GL objects created during rendering.
    */
void VASTOscilloscopeOGL2D::openGLContextClosing()
{
    shader->release();
    shader = nullptr;
    uniforms = nullptr;
}
        
/** The OpenGL rendering callback.
    */
void VASTOscilloscopeOGL2D::renderOpenGL()
{
    jassert (OpenGLHelpers::isContextActive());
    
    // Setup Viewport
	const float renderingScale = (float) openGLContext.getRenderingScale();
	float width = getScreenBounds().getWidth();		
	float height = getScreenBounds().getHeight();
    glViewport (0, 0, roundToInt (renderingScale * width), roundToInt (renderingScale * height));
        
    // Set background Color
    OpenGLHelpers::clear (myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground));
        
    // Enable Alpha Blending
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);       
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(2.0f);
       
	shader->use();

    // Setup the Uniforms for use in the Shader
	if (uniforms->osciColour != nullptr)
		//uniforms->osciColour->set((GLfloat)(m_osciColour.getRed() / 256.0), (GLfloat)(m_osciColour.getGreen() / 256.0), (GLfloat)(m_osciColour.getBlue() / 256.0), (GLfloat)(m_osciColour.getAlpha() / 256.0));
		uniforms->osciColour->set((GLfloat)(m_osciColour.getRed() / 256.0), (GLfloat)(m_osciColour.getGreen() / 256.0), (GLfloat)(m_osciColour.getBlue() / 256.0), (GLfloat)(m_osciColour.getAlpha() / 512.0));
		

    //if (uniforms->resolutionXY != nullptr)
      //  uniforms->resolutionXY->set ((GLfloat) renderingScale * getWidth(), (GLfloat) renderingScale * getHeight());
        
    // Read in samples from ring buffer
    //if (uniforms->audioSampleData != nullptr)
    //{
        //ringBuffer->get()->readSamples(readBuffer, RING_BUFFER_READ_SIZE);
		//ringBuffer->get()->readSamplesFIFO(readBuffer, RING_BUFFER_READ_SIZE / 2);		
		
		int readSize = myEditor->getProcessor()->m_pVASTXperience.m_Set.m_nSampleRate * 0.25; //1/4s
		//int visualSize = OGL2D_VISUAL_SIZE; //todo real gui size
		ringBuffer->get()->readSamplesInterpolated(readBuffer, readSize, OGL2D_VISUAL_SIZE);
            
		//find zero crossing		
		//int crossing = 0;
		/*
		const float* rb = readBuffer.getReadPointer(0, 0);
		for (int i = 0; i < RING_BUFFER_READ_SIZE / 2; i++) {
			if ((rb[i] > 0.f) && (rb[i + 1] < 0.f)) {
				crossing = i;
				break;
			}
		}
		*/

		//FloatVectorOperations::clear(visualizationBuffer, OGL2D_VISUAL_SIZE);

		// Sum channels together
		//for (int i = 0; i < 2; ++i)
		//for (int i = 0; i < 1; ++i)
		//{
			//FloatVectorOperations::add(visualizationBuffer, readBuffer.getReadPointer(i, crossing), OGL2D_VISUAL_SIZE);
		//}
        //uniforms->audioSampleData->set (visualizationBuffer, OGL2D_VISUAL_SIZE);
    //}
     
	/*
    // Define Vertices for a Square (the view plane)
    GLfloat vertices[] = {
        1.0f,   1.0f,  0.0f,  // Top Right
        1.0f,  -1.0f,  0.0f,  // Bottom Right
        -1.0f, -1.0f,  0.0f,  // Bottom Left
        -1.0f,  1.0f,  0.0f   // Top Left
    };
    // Define Which Vertex Indexes Make the Square
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3,   // First Triangle
        1, 2, 3    // Second Triangle
    };
	*/
        
	const float* readBufferPointer = readBuffer.getReadPointer(0);
	for (int i = 0; i < OGL2D_VISUAL_SIZE; i++) {
		oscivertices[i * 2 + 1] = readBufferPointer[i];
	}

	//GLsizeiptr vert_size = static_cast<GLsizeiptr>(2 * OGL2D_VISUAL_SIZE * sizeof(float));
	//openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, vert_size,
		//oscivertices, GL_STATIC_DRAW);

	openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VBO (Vertex Buffer Object) - Bind and Write to Buffer
    openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, VBO);
    openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER, sizeof(oscivertices), oscivertices, GL_STREAM_DRAW);
                                                                // GL_DYNAMIC_DRAW or GL_STREAM_DRAW
                                                                // Don't we want GL_DYNAMIC_DRAW since this
                                                                // vertex data will be changing alot??
                                                                // test this
	openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Use Shader Program that's been defined

	openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // EBO (Element Buffer Object) - Bind and Write to Buffer
	openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(osciindices), osciindices, GL_STREAM_DRAW);
                                                                // GL_DYNAMIC_DRAW or GL_STREAM_DRAW
                                                                // Don't we want GL_DYNAMIC_DRAW since this
                                                                // vertex data will be changing alot??
                                                                // test this
        
    // Setup Vertex Attributes
    openGLContext.extensions.glVertexAttribPointer (position_->attributeID, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    openGLContext.extensions.glEnableVertexAttribArray (position_->attributeID); 
       
	//DRAW
	// Draw Vertices
		//glDrawArrays (GL_TRIANGLES, 0, 6); // For just VBO's (Vertex Buffer Objects)
		//glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // For EBO's (Element Buffer Objects) (Indices)

	glDrawElements (GL_LINES,OGL2D_VISUAL_SIZE * 2, GL_UNSIGNED_INT, 0); // Lines
	//DRAW

	openGLContext.extensions.glDisableVertexAttribArray(position_->attributeID);

    // Reset the element buffers so child Components draw correctly
    openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
    openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisable(GL_LINE_SMOOTH);    
}
    
    
//==========================================================================
// JUCE Callbacks
    
void VASTOscilloscopeOGL2D::paint (Graphics& g) 
{
	if (m_bContRepaint == false) {
		g.fillAll(Colour(0xff0b0b0b));
        if (isVisible()) {
            start();
            m_bContRepaint = true;
        }
	}	
}
    
void VASTOscilloscopeOGL2D::resized () 
{
    //statusLabel.setBounds (getLocalBounds().reduced (4).removeFromTop (75));
}
    
//==========================================================================
// OpenGL Functions
    
    
/** Loads the OpenGL Shaders and sets up the whole ShaderProgram
*/
void VASTOscilloscopeOGL2D::createShaders()
{
    vertexShader =
    "attribute " JUCE_MEDIUMP " vec4 position;\n"
    "\n"
    "void main()\n"
    "{\n"    
	"    gl_Position = position;\n"
    "}\n";
        
    fragmentShader =
	"uniform vec4 osciColour;\n"
    //"uniform vec2 resolutionXY;\n"
    "\n"     		 	  	  
    //"#define THICKNESS 0.06\n"
	"#define THICKNESS 0.02\n"
    "void main()\n"
    "{\n"
    //"    float y = gl_FragCoord.y / resolutionXY.y;\n"
	//"    float x = gl_FragCoord.x / resolutionXY.x;\n"
    //"\n"
    // Centers & Reduces Wave Amplitude
    //"    float amplitude = 0.5 - x / 2.5;\n"
    //"    float r = abs (THICKNESS / (amplitude-x));\n"
    //"\n"
	//"	gl_FragColor = gl_FragColor *  vec4(0.3, 0.3, 0.3, 1.0);\n"	
	//"	gl_FragColor = vec4 (r - abs (r * 0.2), r - abs (r * 0.2), r - abs (r * 0.2), 0.6) * osciColour;\n"		
		//"	gl_FragColor = vec4(r, r, r , 0.6) * osciColour; \n"			
	//" vec4 lColor = osciColour;\n"
	//" float r = abs(position_out.x - gl_FragCoord.x);\n"
	//" float p1 = (position_out.x * 2.0) - 1.0;\n"
	//" float r = abs(x - p1);\n"
	//"		if (r > 0.5)\n"
	//"			 lColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"    gl_FragColor = osciColour;\n"
    "}\n";
        
    std::unique_ptr<OpenGLShaderProgram> newShader (new OpenGLShaderProgram (openGLContext));
    String statusText;
        
    if (newShader->addVertexShader (OpenGLHelpers::translateVertexShaderToV3 (vertexShader))
        && newShader->addFragmentShader (OpenGLHelpers::translateFragmentShaderToV3 (fragmentShader))
        && newShader->link())
    {
        uniforms = nullptr;
            
        shader = std::move(newShader);
        shader->use();
            
        uniforms   = std::make_unique<Uniforms>(openGLContext, *shader);
            
        statusText = "GLSL: v" + String (OpenGLShaderProgram::getLanguageVersion(), 2);
    }
    else
    {
        statusText = newShader->getLastError();
    }
      
	position_ = std::make_unique<OpenGLShaderProgram::Attribute>(*shader, "position");

	//TODO
    //statusLabel.setText (statusText, dontSendNotification);
}
    
