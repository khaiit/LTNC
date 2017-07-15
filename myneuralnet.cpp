#include "myneuralnet.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>






/***************************object functions trong myneuralnet**************/
myneuralnet::myneuralnet():m_hiddenlayers(0),m_hiddenlayercount(0)
{
}

myneuralnet::~myneuralnet()
{
    if(m_hiddenlayers)
    {
        for(int i=0;i<m_hiddenlayercount;i++)
        {
            delete m_hiddenlayers[i];
        }

        delete [] m_hiddenlayers;
    }

}

void myneuralnet::create(int inputcount, int inputneurons, int outputcount, int *hiddenlayers, int hiddenlayercount)
{
   //Dam bao cac gia tri can thiet != 0
            assert(inputcount && inputneurons && outputcount);
            int i;
            m_inputlayer.create(inputcount,inputneurons);
            if(hiddenlayers && hiddenlayercount)
            {
                m_hiddenlayers=new layer*[hiddenlayercount];
                m_hiddenlayercount=hiddenlayercount;
                for(i=0;i<hiddenlayercount;i++)
                {
                    m_hiddenlayers[i]=new layer;
                    if(i==0)
                    {
                    
                        m_hiddenlayers[i]->create(inputneurons,hiddenlayers[i]);
                    }
                    else
                    {
                        m_hiddenlayers[i]->create(hiddenlayers[i-1],hiddenlayers[i]);
                    }
                }
                m_outputlayer.create(hiddenlayers[hiddenlayercount - 1],outputcount);
            }
            else
            {
                m_outputlayer.create(inputneurons,outputcount);
            }
}


void myneuralnet::propagate(const float *input)
{
    //Ham lan truyen bat dau tu input
    

    memcpy(m_inputlayer.layerinput,input,m_inputlayer.inputcount * sizeof(float));
    //tinh inputlayer
    m_inputlayer.calculate();

    update(-1);//truyen qua lop ke tiep
    if(m_hiddenlayers)
    {
        //tinh lop an (neu co)
        for(int i=0;i<m_hiddenlayercount;i++)
        {
            m_hiddenlayers[i]->calculate();
            update(i);
        }
    }

    //tinh output
    m_outputlayer.calculate();
}



//ham training 
float myneuralnet::train(const float *desiredoutput, const float *input, float alpha, float momentum)
{
    
    float errorg=0; //Tong binh phuong sai so
    float errorc; //sai so cuc bo;
    float sum=0,csum=0;
    float delta,udelta;
    float output;
    //Bat dau bang cach truyen di input
    propagate(input);
    int i,j,k;
    //thuat toan lan truyen nguoc bat dau tu output va truyen nuoc ve cac lop tren
    for(i=0;i<m_outputlayer.neuroncount;i++)
    {
        //.tinh sai so
        output=m_outputlayer.neurons[i]->output; 
        errorc=(desiredoutput[i] - output) * output * (1 - output);
        //tinh binh phuong sai so 
        errorg+=(desiredoutput[i] - output) * (desiredoutput[i] - output) ;

        //Cap nhat ca trong so cua neural
        for(j=0;j<m_outputlayer.inputcount;j++)
        {
            //lay gia tri delta hien gio
            delta=m_outputlayer.neurons[i]->deltavalues[j];
            //cap nhat gia tri delta
            udelta=alpha * errorc * m_outputlayer.layerinput[j] + delta * momentum;
            //cap nhat cac trong so
            m_outputlayer.neurons[i]->weights[j]+=udelta;
            m_outputlayer.neurons[i]->deltavalues[j]=udelta;

            //luu lai de truyen qua lop sau
            sum+=m_outputlayer.neurons[i]->weights[j] * errorc;
        }

        //tinh trong so gain
        m_outputlayer.neurons[i]->wgain+= alpha * errorc * m_outputlayer.neurons[i]->gain;

    }

    for(i=(m_hiddenlayercount - 1);i>=0;i--)
    {
        for(j=0;j<m_hiddenlayers[i]->neuroncount;j++)
        {
            output=m_hiddenlayers[i]->neurons[j]->output;
            //tinh sai so trong lop nay
            errorc= output * (1-output) * sum;
            //cap nhat cac trong so neural
            for(k=0;k<m_hiddenlayers[i]->inputcount;k++)
            {
                delta=m_hiddenlayers[i]->neurons[j]->deltavalues[k];
                udelta= alpha * errorc * m_hiddenlayers[i]->layerinput[k] + delta * momentum;
                m_hiddenlayers[i]->neurons[j]->weights[k]+=udelta;
                m_hiddenlayers[i]->neurons[j]->deltavalues[k]=udelta;
                csum+=m_hiddenlayers[i]->neurons[j]->weights[k] * errorc;//dung cho lop sau

            }

            m_hiddenlayers[i]->neurons[j]->wgain+=alpha * errorc * m_hiddenlayers[i]->neurons[j]->gain;

        }
        sum=csum;
        csum=0;
    }

    //cuoi cung la lop input
    for(i=0;i<m_inputlayer.neuroncount;i++)
    {
        output=m_inputlayer.neurons[i]->output;
        errorc=output * (1 - output) * sum;

        for(j=0;j<m_inputlayer.inputcount;j++)
        {
            delta=m_inputlayer.neurons[i]->deltavalues[j];
            udelta=alpha * errorc * m_inputlayer.layerinput[j] + delta * momentum;
            //cap nhat trong so
            m_inputlayer.neurons[i]->weights[j]+=udelta;
            m_inputlayer.neurons[i]->deltavalues[j]=udelta;
        }
        //cap nhat trong so gain
        m_inputlayer.neurons[i]->wgain+=alpha * errorc * m_inputlayer.neurons[i]->gain;
    }

    //tra ve tongbinhphuong sai so /2
    return errorg / 2;

}

void myneuralnet::update(int layerindex)
{
    int i;
    if(layerindex==-1)
    {
        //xu ly inputlayer va truyen qua lop sau
        for(i=0;i<m_inputlayer.neuroncount;i++)
        {
            if(m_hiddenlayers)//trueyn qua lop an dau tien
            {
                m_hiddenlayers[0]->layerinput[i]=m_inputlayer.neurons[i]->output;
            }
            else //truyen truc tiep qua lop output
            {
                m_outputlayer.layerinput[i]=m_inputlayer.neurons[i]->output;
            }
        }

    }
    else
    {
        for(i=0;i<m_hiddenlayers[layerindex]->neuroncount;i++)
        {
            //neu khong phai lop an cuoi cung
            if(layerindex < m_hiddenlayercount -1)
            {
                m_hiddenlayers[layerindex + 1]->layerinput[i]=m_hiddenlayers[layerindex]->neurons[i]->output;
            }
            else
            {
                m_outputlayer.layerinput[i]=m_hiddenlayers[layerindex]->neurons[i]->output;
            }
        }
    }
}


