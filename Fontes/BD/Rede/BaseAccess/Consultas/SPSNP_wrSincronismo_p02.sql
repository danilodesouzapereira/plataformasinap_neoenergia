UPDATE
  (SELECT * FROM Sincronismo) sel
SET
  sel.strCodigo  = [p_Codigo],
  sel.bolEnabled = [p_Enabled]
WHERE
  sel.intSincronismoId = [p_SincronismoId];


