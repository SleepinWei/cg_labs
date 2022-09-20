### Project 3 Extended Kalman Filter

#### 任务描述

我们假设一个轮式机器人在2D平面上运动，且机器人搭载了GPS接收机，每隔0.1s可以获取机器人当前的位置信息[x, y]。轮式机器人自身的控制系统每隔0.1s更新一次运动信息（前进速度与偏航角速度[v, yaw_rate]）以此指导机器人的运动。规定使用2D坐标、朝向，速度来描述机器人的状态：[x, y, yaw, v]。现通过卡尔曼滤波的方法，以机器人的运动指令和GPS观测值为输入，来对机器人进行状态估计。
已提供代码框架（python），需要同学们完成核心代码，主要包括**雅可比矩阵的构建**和**EKF过程**。

#### 算法概述
+ Notion

  robot state: $\textbf{x} = [x, y, \theta,v]^{T}$
  
  control vector: $\textbf{u} = [v,\omega]^T$，$\omega$表示偏航角速度

+ **机器人运动模型**

  $x_{k+1} = x_k + v_k\delta t \cos(\theta_k)$

  $y_{k+1} = y_k + v_k\delta t \sin(\theta_k)$

  $\theta_{k+1} = \theta_k + \omega \delta t$

  $v_{k+1} = v_k$

  注意以上是理想情况，实际运动感模型会产生误差，我们使用一个服从零均值的高斯分布表示噪声，使用矩阵形式表示：

  $\mathbf{x_{k}} = F \mathbf{x_{k-1}}+B \mathbf{u_k} + m_k$

  $F = \begin{bmatrix}1&0&0&0 \\ 0&1&0&0 \\ 0&0&1&0 \\ 0&0&0&0 \end{bmatrix}$ $B = \begin{bmatrix} \delta t \cos(\theta)&0 \\\delta t \sin(\theta)&0  \\ 0&\delta t \\ 1&0 \end{bmatrix}$ 并且$m_k \sim N(0,Q)$

  可以看到机器人运动模型是非线性的，这也是我们为什么使用拓展卡尔曼滤波估计位姿的原因。我们需要对该运动模型进行线性化近似。

+ GPS观测模型

  $\mathbf{z_k} = H\mathbf{x_k} + n_k$ 

  $H  = \begin{bmatrix} 1&0&0&0 \\ 0&1&0&0 \end{bmatrix}$ 并且$n_k \sim N(0,R)$

  GPS观测模型为线性模型，不需要近似。

+ 拓展卡尔曼滤波

  + 传统卡尔曼滤波

    对于一个线性高斯系统
    $$
    \left\{\begin{array}{l}
    \boldsymbol{x}_{k}=\boldsymbol{A}_{k} \boldsymbol{x}_{k-1}+\boldsymbol{u}_{k}+\boldsymbol{w}_{k} \\
    \boldsymbol{z}_{k}=\boldsymbol{C}_{k} \boldsymbol{x}_{k}+\boldsymbol{v}_{k}
    \end{array} \quad k=1, \ldots, N\right.
    $$
    其中：
    $$
    \boldsymbol{w}_{k} \sim N(\mathbf{0}, \boldsymbol{R}) . \quad \boldsymbol{v}_{k} \sim N(\mathbf{0}, \boldsymbol{Q})
    $$
    

    1. 预测

    $$
    \begin{array}{l}
    \tilde{X}_{k+1}=A \hat{X}_{k}+B u_{k} \\
    \tilde{P}_{k+1}=A \hat{P}_{k} A^{T}+Q
    \end{array}
    $$

    2. 计算卡尔曼增益

    $$
    K=C \tilde{P}_{k+1} C^{T} /\left(C \tilde{P}_{k+1} C^{T}+R\right)
    $$

    3. 更新

    $$
    \begin{array}{c}
    \hat{X}_{k+1}=\tilde{X}_{k+1}+K\left(Z_{k+1}-C \tilde{X}_{k+1}\right) \\
    \hat{P}_{k+1}=(I-K C) \tilde{P}_{k+1}
    \end{array}
    $$

  + 对于非线性系统，需要先将非线性函数进行泰勒展开，一般情况下取一阶项进行线性近似。
    $$
    x_{k}=f\left(x_{k-1}, u_{k}\right)+w_{k} \\ z_{k}=h\left(x_{k}\right)+v_{k}, k=1, \ldots, N
    $$
    
    $$
    \begin{array}{c}
    x_{k} \approx f\left(\hat{x}_{k-1}, u_{k-1}\right)+\frac{\partial f}{\partial x_{k-1}}\left(x_{k-1}-\hat{x}_{k-1}\right)+w_{k} \\
    F=\frac{\partial f}{\partial x_{k-1}} \\
    z_{k} \approx h\left(\hat{x}_{k}\right)+\frac{\partial h}{\partial x_{k}}\left(x_{k}-\hat{x}_{k-1}\right)+n_{k} \\
    H=\frac{\partial h}{\partial x_{k}}
    \end{array}
    $$
    拓展卡尔曼滤波与卡尔曼滤波非常相似，注意其中矩阵F与H的使用。
    $$
    \overline{\boldsymbol{x}}_{k}=f\left(\hat{\boldsymbol{x}}_{k-1}, \boldsymbol{u}_{k}\right), \quad \\ \overline{\boldsymbol{P}}_{k}=\boldsymbol{F} \hat{\boldsymbol{P}}_{k} \boldsymbol{F}^{T}+\boldsymbol{R}_{k} \\
    \boldsymbol{K}_{k}=\overline{\boldsymbol{P}}_{k} \boldsymbol{H}^{\mathrm{T}}\left(\boldsymbol{H} \overline{\boldsymbol{P}}_{k} \boldsymbol{H}^{\mathrm{T}}+\boldsymbol{Q}_{k}\right)^{-1} \\
    \hat{\boldsymbol{x}}_{k}=\overline{\boldsymbol{x}}_{k}+\boldsymbol{K}_{k}\left(\boldsymbol{z}_{k}-h\left(\overline{\boldsymbol{x}}_{k}\right)\right) \\ \hat{\boldsymbol{P}}_{k}=\left(\boldsymbol{I}-\boldsymbol{K}_{k} \boldsymbol{H}\right) \overline{\boldsymbol{P}}_{k}
    $$
    

+ 线性化近似（求雅可比矩阵)

  对于一个非线性函数$\mathbf{y}=f(\mathbf{x}) \in \mathbb{R}_{m}$，其中$\mathbf{x} \in \mathbb{R}_{n}$

  可以将其展开
  $$
  \left\{\begin{array}{l}
  y_{1}=f_{1}\left(x_{1}, \ldots, x_{n}\right) \\
  y_{2}=f_{2}\left(x_{1}, \ldots, x_{n}\right) \\
  \cdots \\
  y_{m}=f_{m}\left(x_{1}, \ldots, x_{n}\right)
  \end{array}\right.
  $$
  

  那么其雅可比矩阵可以表示为一个m*n的矩阵：
  $$
  \frac{\partial \mathbf{f}} {\mathbf{\partial x}} =  \mathbf{J}=\left[\begin{array}{lll}
  \frac{\partial \mathbf{f}}{\partial x_{1}} & \cdots & \frac{\partial \mathbf{f}}{\partial x_{n}}
  \end{array}\right]=\left[\begin{array}{ccc}
  \frac{\partial f_{1}}{\partial x_{1}} & \cdots & \frac{\partial f_{1}}{\partial x_{n}} \\
  \vdots & \ddots & \vdots \\
  \frac{\partial f_{m}}{\partial x_{1}} & \cdots & \frac{\partial f_{m}}{\partial x_{n}}
  \end{array}\right]
  $$
  回忆运动模型中的非线性函数，求取对应的雅可比矩阵，作为运动模型的线性近似。

#### 参考

+ SLAM十四讲10.1 p238