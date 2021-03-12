    UPDATE
      (SELECT * FROM Bateria) sel
    SET
      sel.intTipoFaseId     = [p_TipoFaseId],
	  sel.intTipoOperacaoId = [p_TipoOperacaoId],
      sel.fltIctepu         = [p_IctePu], 
      sel.fltZctepu         = [p_ZctePu], 
      sel.fltSctepu         = [p_SctePu], 
	  sel.intModeloCarga    = [p_ModeloCarga], 
	  sel.intModeloDescarga = [p_ModeloDescarga],
	  sel.fltPnomkW         = [p_PnomkW], 
	  sel.fltEnomkWh        = [p_EnomkWh], 
	  sel.fltCapinvkVA      = [p_CapinvkVA], 
	  sel.fltPociosapu      = [p_Pociosapu], 
	  sel.fltQociosapu      = [p_Qociosapu], 
	  sel.fltRendCargapu    = [p_RendCargapu], 
	  sel.fltRendDescpu     = [p_RendDescpu], 
	  sel.fltGatilhoCargapu = [p_GatilhoCargapu],
	  sel.fltGatilhoDescpu  = [p_GatilhoDescpu]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intBateriaId = [p_BateriaId];

      
      