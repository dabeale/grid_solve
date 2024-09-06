# Grid Solve

Software for solving forward and inverse problems on a grid. Examples include,

 * Fluid Simulation
 * Surface Reconstruction
 * Approximate Matrix Inversion / Multiplication
 * The Poisson Equation

## Run the tests

Build the tests and run them in docker as follows,

```bash 
docker build . -t grid_solve
docker run --rm grid_solve /test/bin/test_release
```

To build them locally, use `make all`, but ensure that gcc is installed with a major version of at least 11.

<p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/"><a property="dct:title" rel="cc:attributionURL" href="https://github.com/dabeale/grid_solve">grid_solve</a> by <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://github.com/dabeale">Daniel Beale</a> is licensed under <a href="https://creativecommons.org/licenses/by-nc-sa/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">CC BY-NC-SA 4.0<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1" alt=""><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1" alt=""><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/nc.svg?ref=chooser-v1" alt=""><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/sa.svg?ref=chooser-v1" alt=""></a></p>
