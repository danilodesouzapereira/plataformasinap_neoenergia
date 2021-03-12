  UPDATE 
    (SELECT * FROM Estudo) sel  
  SET 
    sel.tmsModificacao = Now()
  WHERE 
    sel.intEstudoId = [p_EstudoId];
