#include <iostream>
#include "TGraph.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TAxis.h"

void vida_particulas(){
    //Datos experimentales
    const int n = 20;
    double t[n] = {62, 68, 73, 85, 90, 97, 101, 105, 112, 120, 125, 130, 138, 144, 149, 156, 163, 170, 175, 180};
    double A[n] = {592, 527, 510, 431, 380, 353, 318, 310, 290, 242, 215, 208, 187, 177, 158, 142, 125, 110, 109, 100};
    //Graficar los puntitos
    TGraph *gr = new TGraph(n, t, A);
    gr->SetTitle("Decaimiento Radiactivo;Tiempo (min);Actividad (desint/min)");
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.0);
    gr->SetMarkerColor(kBlue + 1);
    //A(t) = A0 * exp(-lambda * t)
    TF1 *fit_func = new TF1("fit_func", "[0]*exp(-[1]*x)", 60, 180);
    fit_func->SetParNames("A_0", "lambda");
    fit_func->SetParameter(0, 1000.0);  // Estimación inicial razonable de A0
    fit_func->SetParameter(1, 0.015);   // Estimación inicial de lambda
    fit_func->SetLineColor(kRed);
    fit_func->SetLineWidth(2);
    //Ajuste de la funcion
    gr->Fit(fit_func, "R"); // "R" respeta el rango definido en TF1

    // 5. Extraer los parámetros del ajuste y sus errores
    double A0 = fit_func->GetParameter(0);
    double err_A0 = fit_func->GetParError(0);

    double lambda = fit_func->GetParameter(1);
    double err_lambda = fit_func->GetParError(1);

    // Cálculo det_1/2 = ln(2) / lambda
    double t_medio = TMath::Log(2) / lambda;
    // Propagación de error: delta(t_1/2) = (ln(2) / lambda^2) * delta(lambda)
    double err_t_medio = (TMath::Log(2) / (lambda * lambda)) * err_lambda;

    // 6. Mostrar resultados
    std::cout << "\n================ RESULTADOS ================" << std::endl;
    std::cout << "Actividad inicial estimada (A0): " << A0 << " +/- " << err_A0 << " desint/min" << std::endl;
    std::cout << "Constante de decaimiento (lambda): " << lambda << " +/- " << err_lambda << " min^-1" << std::endl;
    std::cout << "Vida media (t_1/2): " << t_medio << " +/- " << err_t_medio << " min" << std::endl;
    std::cout << "============================================" << std::endl;

    // 7. Dibujar el gráfico
    TCanvas *c1 = new TCanvas("c1", "Ajuste de Decaimiento Radiactivo", 800, 600);
    c1->SetGrid();
    gr->Draw("AP");
}