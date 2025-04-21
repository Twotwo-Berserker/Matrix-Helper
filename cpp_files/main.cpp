#include "../head_files/Matrix.h"
#include "../head_files/ComplexMatrix.h"
// #include "../head_files/LinearSolver.h"
// #include "../head_files/NormCalculator.h"
#include<iostream>
using namespace std;

void MainMenu();
void exit();
void FindFile(const string& filename);
void op1();
void op2();
void op3();
void op4();
void op5();
void op6();
void op7();

int main() {
    cout<<"感谢使用本程序！\n我们致力于帮助用户解决各种矩阵、向量和线性方程组问题。\n";
    cout<<"目前只支持实矩阵的存入、加减乘运算、转置\n";
    MainMenu();
    return 0;
}

void MainMenu(){
    cout<<"请选择您需要的操作：\n";
    cout<<"1. 矩阵管理\n";
    cout<<"2. 矩阵的加减乘\n";
    cout<<"3. 矩阵的转置\n";
    cout<<"4. 矩阵的秩\n";
    cout<<"5. 矩阵的行列式\n";
    cout<<"6. 矩阵的逆\n";
    cout<<"7. 伴随矩阵\n";
    cout<<"10. 退出\n";
    int operation;
    cin>>operation;
    if(operation==1) op1();
    else if(operation==2) op2();
    else if(operation==3) op3();
    else if(operation==4) op4();
    else if(operation==5) op5();
    else if(operation==6) op6();
    else if(operation==7) op7();

    else if(operation==10){
        cout<<"感谢使用本程序！\n";
        system("pause");
        return;
    }
    else{
       exit();
       return;
    }
    system("pause");
    MainMenu();
}

void exit(){
    cout<<"输入错误，程序正在退出...\n";
    system("pause");
}

void op1(){
    cout<<"请选择你需要的操作：\n";
        cout<<"1. 矩阵存入\n";
        cout<<"2. 矩阵重命名\n";
        cout<<"3. 矩阵删除\n";
        cout<<"4. 查询矩阵\n";
        cout<<"5. 化为最简型\n";
        cout<<"6. 返回主菜单\n";
        int choice;
        cin>>choice;
        try{
            if(choice==1){
                cout<<"请输入矩阵类型：\n";
                cout<<"1. 实矩阵\n";
                cout<<"2. 复矩阵\n";
                int type;
                cin>>type;
                if(type!=1 && type!=2) {
                    exit();
                    return;
                }
                cout<<"请输入矩阵的大小：\n";
                cout<<"r: ";
                int r;
                cin>>r;
                cout<<"c: ";
                int c;
                cin>>c;
                cout<<"请输入矩阵名：\n";
                string filename;
                cin>>filename;
                if(type==1){
                    Matrix<double> A(r,c);
                    for(int i=0;i<r;i++){
                        for(int j=0;j<c;j++){
                            cout<<filename<<"("<<i<<","<<j<<"): ";
                            cin>>A.data[i][j];
                        }
                    }
                    A.saveToFile("docs/"+filename);
                }
                else if(type==2){
                    ComplexMatrix<double> A(r,c);
                    for(int i=0;i<r;i++){
                        for(int j=0;j<c;j++){
                            cout<<filename<<"("<<i<<","<<j<<"): ";
                            double real,imag;
                            cin>>real>>imag;
                            complex<double> tmp(real,imag);
                            A.data[i][j]=tmp; 
                            cout<<"tmp.real="<<A.data[i][j].real()<<endl;
                            cout<<"tmp.imag="<<A.data[i][j].imag()<<endl;
                        }
                    }
                    A.saveToFile("docs/"+filename);
                }
            }
            else if(choice==2){
                cout<<"请输入要重命名的矩阵名：\n";
                string filename;
                cin>>filename;
                FindFile(filename);
                cout<<"请输入新矩阵名：\n";
                string newfilename;
                cin>>newfilename;
                Matrix<double> A("docs/"+filename);
                A.saveToFile("docs/"+newfilename);
                remove(("docs/"+filename).c_str());
            }
            else if(choice==3){
                cout<<"请输入要删除的矩阵名：\n";
                string filename;
                cin>>filename;
                FindFile(filename);
                remove(("docs/"+filename).c_str());
                cout<<"矩阵删除成功！\n";
            }
            else if(choice==4){
                cout<<"已存储的矩阵如下：\n";
                system("dir docs /b");
                cout<<"请输入要查询的矩阵名：\n";
                string filename;
                cin>>filename;
                FindFile(filename);
                cout<<"矩阵的类型：\n";
                cout<<"1. 实矩阵\n";
                cout<<"2. 复矩阵\n";
                int type;
                cin>>type;  
                if(type==1) {
                    Matrix<double> A("docs/"+filename);
                    A.print();
                }
                else {
                    ComplexMatrix<double> A("docs/"+filename);
                    A.print();
                }
            }
            else if(choice==5){
                cout<<"请输入矩阵名：\n";
                string filename;
                cin>>filename;
                FindFile(filename);
                Matrix<double> A("docs/"+filename);
                A.reducedRowEchelonForm();
                cout<<"1. 覆盖原矩阵\n";
                cout<<"2. 另存为：\n";
                int choice;
                cin>>choice;
                if(choice==1)
                    A.saveToFile("docs/"+filename);
                else if(choice==2)
                {
                    cout<<"请输入新矩阵名：\n";
                    string newfilename;
                    cin>>newfilename;
                    A.saveToFile("docs/"+newfilename);
                }
            }
            else{
                exit();
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
}

void op2(){
    try{
        cout<<"请输入第一个矩阵名：\n";
        string filename1;
        cin>>filename1;
        FindFile(filename1);
        Matrix<double> A("docs/"+filename1);
        cout<<"请输入第二个矩阵名：\n";
        string filename2;
        cin>>filename2;
        FindFile(filename2);
        Matrix<double> B("docs/"+filename2);
        cout<<"请输入要进行的操作：\n";
        cout<<"1. 加法\n";
        cout<<"2. 减法\n";
        cout<<"3. 乘法\n";
        int choice;
        cin>>choice;
        Matrix<double> C;
        switch(choice)
        {
        case 1:
            C=A+B;
            C.saveToFile("docs/"+filename1+"_add_"+filename2);
            break;
        case 2:
            C=A-B;
            C.saveToFile("docs/"+filename1+"_sub_"+filename2);
            break;
        case 3:
            C=A*B;
            C.saveToFile("docs/"+filename1+"_mul_"+filename2);
            break;
        default:
            break;
        }
    } catch(const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void op3(){
    try{
        cout<<"请输入要转置的矩阵名：\n";
        string filename;
        cin>>filename;
        FindFile(filename);
        Matrix<double> A("docs/"+filename);
        cout<<"是否将原矩阵覆盖？\n";
        cout<<"1. 是\n";
        cout<<"2. 否\n";
        int choice;
        cin>>choice;
        if(choice==1) {
            A.transpose().saveToFile("docs/"+filename);
        }
        else if(choice==2) {
            cout<<"请输入新矩阵名：\n";
            string newfilename;
            cin>>newfilename;
            A.transpose().saveToFile("docs/"+newfilename);
        }
        else{
            exit();
        }
    } catch(const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void op4(){
    try{
        cout<<"请输入矩阵名：\n";
        string filename;
        cin>>filename;
        FindFile(filename);
        Matrix<double> A("docs/"+filename);
        cout<< filename <<"矩阵的秩为："<<A.rank()<<endl;
    } catch(const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void op5(){
    try{
        cout<<"请输入矩阵名：\n";
        string filename;
        cin>>filename;
        FindFile(filename);
        Matrix<double> A("docs/"+filename);
        cout<< filename <<"矩阵的行列式为："<<A.determinant()<<endl;
    } catch(const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void op6(){
    try{
        cout<<"请输入矩阵名：\n";
        string filename;
        cin>>filename;
        FindFile(filename);
        Matrix<double> A("docs/"+filename);
        cout<< filename <<"矩阵的逆为："<<endl;
        A.inverse().print();
    } catch(const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void op7(){
    try{
        cout<<"请输入矩阵名：\n";
        string filename;
        cin>>filename;
        FindFile(filename);
        Matrix<double> A("docs/"+filename);
        cout<< filename <<"矩阵的伴随矩阵为："<<endl;
        A.adjugate().print();
    } catch(const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}


void FindFile(const string& filename){
    ifstream file("docs/"+filename);
    if(file.is_open()){
        file.close();
    }
    else{
        throw invalid_argument(filename+"文件不存在！");
    }
}
