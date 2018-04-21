# SpeciFlex

SpeciFlex comes in two flavors, `SpeciFlex_solo` and `SpeciFlex_compare`. `SpeciFlex_solo` is used to analyze a single trajectory, whereas `SpeciFlex_compare` can be used to compare two trajctories to each other (i.e., computing their difference-volume).

## More information about SpeciFlex

Please refer to the SpeciFlex manuscript for more information about the theory behind SpeciFlex:

- Matthew E. Tonero, Leslie A. Kuhn. *SpeciFlex: A Protocol for Mining Binding Site Conformational Selectivity in Structure-Based Inhibitor Discovery* bioRxiv 304931; doi: [https://doi.org/10.1101/304931](https://doi.org/10.1101/304931)


## Installing SpeciFlex

To compile the SpeciFlex C code, just run the following commands to generate the executable (standalone) programs for your OS (macOS or Linux):

    gcc -O2 SpeciFlex_compare.c -o SpeciFlex_compare
    gcc -O2 SpeciFlex_solo.c -o SpeciFlex_solo

## Using SpeciFlex


### SpeciFlex_compare

The following usage example assumes that `SpeciFlex_compare` was installed in a user directory `/home/toneroma/SpeciFlex/`):

    /home/toneroma/SpeciFlex/SpeciFlex_compare EC \
    /home/toneroma/HPPK/clustering/MD/EC_suli_noNA.pdb \
    /home/toneroma/HPPK/clustering/MD/EC_REM_3_to_5_w_xtal.crd \
    YP /home/toneroma/HPPK/clustering/MD/YP_dali_align_fixnames.pdb \
    /home/toneroma/HPPK/clustering/MD/YP_REM_superimpose_align.crd \
    /home/toneroma/HPPK/clustering/MD/1q0n/1q0n_lig.pdb 0.5 7.0 1.4 0.25 0

The command above will compare the 2 trajectories 

-  with a 0.5 grid spacing,
-  place a shell of 7 angstroms around the ligand for the grid,
-  use a probe radius of 1.4 angstroms,
-  require the primary protein to have an available volume at least 25% of the time,

while never having an available volume in the secondary protein (both comparisons exist in the compare.pdb file, with the positive values being for volumes in the primary protein and negative values for volumes in the secondary protein).

### SpeciFlex_solo

The following usage example assumes that `SpeciFlex_solo` was installed in a user directory `/home/toneroma/SpeciFlex/`):

    /home/toneroma/SpeciFlex/SpeciFlex_compare EC \
    /home/toneroma/HPPK/clustering/MD/EC_suli_noNA.pdb \
    /home/toneroma/HPPK/clustering/MD/EC_REM_3_to_5_w_xtal.crd \
    /home/toneroma/HPPK/clustering/MD/1q0n/1q0n_lig.pdb 0.5 7.0 1.4

This will only create the ascii grid (`.agd`) and `.pdb` files for the one trajectory. Whenever this is done, it is critical that the `ligand.pdb` file, as well as the grid spacing, shell radius, and probe radius are all EXACTLY the same for all analyzed trajectories. To do further comparison work, a script can be written to handle manipulations of the various output files (i.e. to find where a value of 0 exists in one, but at least a value of 0.25 in the same corresponding grid point).

(Note that there will be some edits needed in order to handle the special residue types (e.g., carboxylated lysine), and other possible format differences.)