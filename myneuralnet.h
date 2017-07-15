#ifndef BPNET_H
#define BPNET_H

/*********************************Cau truc Neural******************************/
struct neuron
{
    float *weights; // Cac trong so dau vao neural
    float *deltavalues; //gia tri delta 
    float output; //output 
    float gain;//Gain 
    float wgain;//Weight gain

    neuron();//Constructor
    ~neuron();//Destructor
    void create(int inputcount);//Cap phat vung nho va khoi tao cac gia tri
};
/**************************************Cau truc lop******************************/
struct layer
{
    neuron **neurons;//dung mang de luu neurons
    int neuroncount;//tong so  neurons
    float *layerinput;//lop input
    int inputcount;//So phan tu trong lop input

    layer();//Khoi tao cac gi tri =0

    ~layer();//Giai phong vung nho

    void create(int inputsize, int _neuroncount);//Tao lop va phan chia vung nho
    void calculate();// Tinh toan cac neural theo cong thuc mong muon
};
/********************************Cau truc mang ********************************/
class myneuralnet
{
private:
    layer m_inputlayer;//lop input cua mang
    layer m_outputlayer;//lop output : chua ket qua
    layer **m_hiddenlayers;//lop an
    int m_hiddenlayercount;/// so lop an

public:
//khoi tao Tao mang
    myneuralnet();//Construction
    ~myneuralnet();//Destructo

    //Tao mang
    void create(int inputcount,int inputneurons,int outputcount,int *hiddenlayers,int hiddenlayercount);

    void propagate(const float *input);//Tinh cac gia tri cua mang dua tren input
    //Cap nhat gia tri cac trong so dua tren ket qua input va ap dung thuat ttoan lan truyen nguoc
    
    float train(const float *desiredoutput,const float *input,float alpha, float momentum);

    //Cap nhat gia tri input o lop sau
    void update(int layerindex);

    //Tra ve output
    inline layer &getOutput()
    {
        return m_outputlayer;
    }

};

#endif
